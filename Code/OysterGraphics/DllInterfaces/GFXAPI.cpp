#include "GFXAPI.h"
#include "../Core/Core.h"
#include "../Render/Resources.h"
#include "../Render/DefaultRenderer.h"
#include "../FileLoader/ObjReader.h"
#include "Resource/ResourceManager.h"
#include "../FileLoader/GeneralLoader.h"
#include "../Model/ModelInfo.h"
#include "../Render/GuiRenderer.h"
#include <vld.h>

namespace Oyster
{
	namespace Graphics
	{
		namespace
		{
			Math::Float4x4 View;
			Math::Float4x4 Projection;
			std::vector<Definitions::Pointlight> Lights;
			float deltaTime;
#ifdef _DEBUG
			Model::Model* cube;
			Model::Model* sphere;

			ID3D11RasterizerState* wire;

			ID3D11ShaderResourceView* debugSRV;
#endif
		}

		API::State API::Init(HWND Window, bool MSAA_Quality, bool Fullscreen, API::Option o)
		{
			Core::resolution = o.Resolution;
			Core::modelPath = o.modelPath;
			Core::texturePath = o.texturePath;

			if(Core::Init::FullInit(Window, MSAA_Quality, Fullscreen) == Core::Init::Fail)
			{
				return API::Fail;
			}
			Render::Resources::Gui::Text::Font = (ID3D11ShaderResourceView*)API::CreateTexture(L"font_generic.png");
			Render::Resources::Init();

			Definitions::PostData pd;
			pd.Amb = o.AmbientValue;

			void* data = Render::Resources::Post::Data.Map();
			memcpy(data,&pd,sizeof(Definitions::PostData));
			Render::Resources::Post::Data.Unmap();

			Render::Preparations::Basic::SetViewPort();
#ifdef _DEBUG
			//fix load model
			
			debugSRV = (ID3D11ShaderResourceView*)API::CreateTexture(L"color_white.png");
			debugSRV = (ID3D11ShaderResourceView*)API::CreateTexture(L"color_white.png");

			cube = CreateModel(L"generic_cube.dan");
			cube->Tint = Math::Float3(0.0f,0.0f,1.0f);
			sphere = CreateModel(L"generic_sphere.dan");
			sphere->Tint = Math::Float3(1.0f,0.5f,182/255.0f);


			D3D11_RASTERIZER_DESC desc;
			desc.CullMode = D3D11_CULL_BACK;
			desc.FillMode = D3D11_FILL_WIREFRAME;
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0;
			desc.DepthClipEnable = true;
			desc.SlopeScaledDepthBias = 0;
			desc.ScissorEnable = false;
			desc.MultisampleEnable = false;
			desc.AntialiasedLineEnable = false;

			Core::device->CreateRasterizerState(&desc,&wire);
#endif
			return API::Sucsess;
		}

		void API::SetProjection(const Math::Float4x4& projection)
		{
			Projection = projection;
		}

		void API::SetView(const Math::Float4x4& view)
		{
			View = view;
		}

		void API::NewFrame()
		{
			if(Lights.size())
			{
				Render::DefaultRenderer::NewFrame(View, Projection, &Lights[0], (int)Lights.size());
			}
			else
			{
				Render::DefaultRenderer::NewFrame(View, Projection, NULL, 0);
			}
		}

		void API::RenderScene(Model::Model models[], int count)
		{
			Render::DefaultRenderer::RenderScene(models,count, View, Projection, deltaTime);
		}

		void API::RenderModel(Model::Model* m)
		{
			Render::DefaultRenderer::RenderScene(m,1, View, Projection, deltaTime);
		}

		void API::EndFrame()
		{
			Render::DefaultRenderer::EndFrame();
		}

		API::State API::SetOptions(API::Option option)
		{
			Core::modelPath = option.modelPath;
			Core::texturePath = option.texturePath;
			
			Definitions::PostData pd;
			pd.Amb = option.AmbientValue;

			void* data = Render::Resources::Post::Data.Map();
			memcpy(data,&pd,sizeof(Definitions::PostData));
			Render::Resources::Post::Data.Unmap();

			return API::Sucsess;
		}

		//returns null for invalid filenames
		Model::Model* API::CreateModel(std::wstring filename)
		{
			Model::Model* m = new Model::Model();
			m->WorldMatrix = Oyster::Math::Float4x4::identity;
			m->Visible = true;
			m->Animation.AnimationPlaying = NULL;
			m->Tint = Math::Float3(1);
			m->info = (Model::ModelInfo*)Core::loader.LoadResource((Core::modelPath + filename).c_str(),Oyster::Graphics::Loading::LoadDAN, Oyster::Graphics::Loading::UnloadDAN);

			Model::ModelInfo* mi = (Model::ModelInfo*)m->info;
			if(!mi || mi->Vertices->GetBufferPointer() == NULL)
			{
				delete m;
				Core::loader.ReleaseResource(mi);
				delete mi;
				return NULL;
			}

			return m;
		}

		void API::DeleteModel(Model::Model* model)
		{
			if(model==NULL)
				return;
			Model::ModelInfo* info = (Model::ModelInfo*)model->info;
			delete model;
			Core::loader.ReleaseResource(info);
		}

		void API::Clean()
		{
#ifdef _DEBUG
			DeleteModel(cube);
			DeleteModel(sphere);
			SAFE_RELEASE(wire);
#endif
			DeleteTexture(Render::Resources::Gui::Text::Font);
			SAFE_DELETE(Core::viewPort);
			Core::loader.Clean();
			Oyster::Graphics::Core::PipelineManager::Clean();
			Oyster::Graphics::Render::Resources::Clean();

			SAFE_RELEASE(Core::depthStencil);
			SAFE_RELEASE(Core::depthStencilUAV);
			SAFE_RELEASE(Core::backBufferRTV);
			SAFE_RELEASE(Core::backBufferUAV);

			SAFE_RELEASE(Core::swapChain);
			SAFE_RELEASE(Core::deviceContext);
			SAFE_RELEASE(Core::device);

		}

		void API::AddLight(Definitions::Pointlight light)
		{
			Lights.push_back(light);
		}

		void API::ClearLights()
		{
			Lights.clear();
		}

#ifdef _DEBUG
		API::State API::ReloadShaders()
		{
			Render::Resources::InitShaders();
			return State::Sucsess;
		}

		void API::StartRenderWireFrame()
		{
			Core::deviceContext->OMSetRenderTargets((UINT)Render::Resources::Gather::Pass.RTV.size(),&Render::Resources::Gather::Pass.RTV[0],NULL);
			Core::deviceContext->RSSetState(wire);
			Core::deviceContext->OMSetRenderTargets((UINT)Render::Resources::Gather::Pass.RTV.size(),&Render::Resources::Gather::Pass.RTV[0],NULL);
		}

		void API::RenderDebugCube(Math::Matrix world)
		{
			cube->WorldMatrix = world;
			Render::DefaultRenderer::RenderScene(cube,1,View,Projection);
		}

		void API::RenderDebugSphere(Math::Matrix world)
		{
			sphere->WorldMatrix = world;
			Render::DefaultRenderer::RenderScene(sphere,1,View,Projection);
		}
#endif

		API::Option API::GetOption()
		{
			Option o;
			o.BytesUsed = Core::UsedMem;
			o.modelPath = Core::modelPath;
			o.texturePath = Core::texturePath;
			o.Resolution = Core::resolution;
			return o;
		}

		void API::StartGuiRender()
		{
			Render::Gui::Begin2DRender();
		}

		void API::RenderGuiElement(API::Texture tex, Math::Float3 pos, Math::Float2 size, Math::Float4 color)
		{
			Render::Gui::Render((ID3D11ShaderResourceView*)tex,pos,size,color);
		}

		API::Texture API::CreateTexture(std::wstring filename)
		{
			return Core::loader.LoadResource((Core::texturePath + filename).c_str(),Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);
		}

		void API::DeleteTexture(API::Texture tex)
		{
			Core::loader.ReleaseResource(tex);
		}

		float API::PlayAnimation(Model::Model* m, std::wstring name,bool looping)
		{
			if(m==NULL)
				return 0;
			m->Animation.AnimationPlaying = &(*m->info->Animations.find(name)).second;
			m->Animation.AnimationTime=0;
			m->Animation.LoopAnimation = looping;
			return (float)m->Animation.AnimationPlaying->duration;
		}

		void API::Update(float dt)
		{
			deltaTime = dt;
		}

		void API::StartTextRender()
		{
			Render::Gui::Begin2DTextRender();
		}

		void API::RenderText(std::wstring text, Math::Float3 Pos, Math::Float2 Size, float FontSize, Math::Float4 color)
		{
			Render::Gui::RenderText(text, Pos, Size, FontSize, color);
		}
	}
}