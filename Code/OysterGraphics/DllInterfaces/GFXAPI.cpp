#include "GFXAPI.h"
#include "../Core/Core.h"
#include "../Render/Resources.h"
#include "../Render/DefaultRenderer.h"
#include "../FileLoader/ObjReader.h"
#include "Resource/ResourceManager.h"
#include "../FileLoader/GeneralLoader.h"
#include "../Model/ModelInfo.h"
#include "../Render/GuiRenderer.h"
#include "Utilities.h" // need StaticArray::NumElementsOf
#include <vld.h>

using ::Utility::StaticArray::NumElementsOf;

namespace Oyster
{
	namespace Graphics
	{
		namespace
		{
			Math::Float4x4 View = Math::Float4x4::identity;
			Math::Float4x4 Projection;
			std::vector<Definitions::Pointlight*> Lights;
			float deltaTime;
			int MostModel;
#ifdef _DEBUG
			Model::Model* cube;
			Model::Model* sphere;

			ID3D11RasterizerState* wire;

			ID3D11ShaderResourceView* debugSRV;
#endif
		}

		API::State API::Init(HWND Window, bool MSAA_Quality, API::Option o)
		{
			Core::resolution = o.resolution;
			Core::modelPath = o.modelPath;
			Core::texturePath = o.texturePath;
			Core::fullscreen = o.fullscreen;

			if(Core::Init::FullInit(Window, MSAA_Quality, o.fullscreen) == Core::Init::Fail)
			{
				return API::Fail;
			}
			Render::Resources::Gui::Text::Font = (ID3D11ShaderResourceView*)API::CreateTexture(L"font_generic.png");
			Render::Resources::Init();

			Definitions::PostData pd;
			Core::amb = pd.Amb = o.ambientValue;
			Core::gGTint = pd.GlowTint = o.globalGlowTint;
			Core::gTint = pd.Tint = o.globalTint;

			void* data = Render::Resources::Post::Data.Map();
			memcpy(data,&pd,sizeof(Definitions::PostData));
			Render::Resources::Post::Data.Unmap();

			Render::Preparations::Basic::SetViewPort();
#ifdef _DEBUG
			//fix load model
			
			debugSRV = (ID3D11ShaderResourceView*)API::CreateTexture(L"color_white.png");
			debugSRV = (ID3D11ShaderResourceView*)API::CreateTexture(L"color_white.png");

			cube = CreateModel(L"generic_cube.dan");
			cube->Tint = Math::Float3(1.0f,0.0f,0.0f);
			cube->Instanced = false;
			sphere = CreateModel(L"generic_sphere.dan");
			sphere->Tint = Math::Float3(1.0f,0.5f,182/255.0f);
			sphere->Instanced = false;


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
			//View.v[0] = -View.v[0];
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
			pd.Amb = option.ambientValue;
			pd.Tint = option.globalTint;
			pd.GlowTint = option.globalGlowTint;

			Core::amb = option.ambientValue;
			Core::gTint = option.globalTint;
			Core::gGTint = option.globalGlowTint;

			void* data = Render::Resources::Post::Data.Map();
			if(data)
			{
				memcpy(data,&pd,sizeof(Definitions::PostData));
				Render::Resources::Post::Data.Unmap();
				Math::Float2 test = Core::resolution;
				if(option.resolution != Core::resolution)
				{
					//RESIZE
					Core::Init::ReInitialize(false,option.fullscreen,option.resolution);
					Render::Resources::ReInitViews(option.resolution);
					Core::resolution = option.resolution;
					Render::Preparations::Basic::SetViewPort();
				}
				if(option.fullscreen != Core::fullscreen)
				{
					Core::swapChain->SetFullscreenState(option.fullscreen, NULL);
					Core::fullscreen = option.fullscreen;
				}
				return API::Sucsess;
			}

			return API::Fail;
		}

		void API::BeginLoadingModels()
		{
		}

		void API::EndLoadingModels()
		{
			//TODO finalize instance buffers and create rendering map;
			int maxModels = 0;
			for(auto i = Render::Resources::RenderData.begin(); i != Render::Resources::RenderData.end(); i++ )
			{
				if((*i).second->Models > maxModels)
				{
					maxModels = (*i).second->Models;
				}
				if( (*i).second->rid != nullptr)
					delete (*i).second->rid;
				(*i).second->rid = new Definitions::RenderInstanceData[(*i).second->Models+1];
			}

			for(auto i = Render::Resources::NoDepthData.begin(); i != Render::Resources::NoDepthData.end(); i++ )
			{
				if((*i).second->Models > maxModels)
				{
					maxModels = (*i).second->Models;
				}
				if( (*i).second->rid != nullptr)
					delete (*i).second->rid;
				(*i).second->rid = new Definitions::RenderInstanceData[(*i).second->Models+1];
			}

			Core::Buffer::BUFFER_INIT_DESC desc;
			

			desc.ElementSize = sizeof(Definitions::RenderInstanceData);
			desc.Type = Core::Buffer::VERTEX_BUFFER;
			desc.Usage = Core::Buffer::BUFFER_CPU_WRITE_DISCARD;
			desc.InitData = 0;
			desc.NumElements = maxModels+1;
			Render::Resources::Gather::InstancedData.~Buffer();
			Render::Resources::Gather::InstancedData.Init(desc);
		}

		//returns null for invalid filenames
		Model::Model* API::CreateModel(std::wstring filename)
		{
			Model::Model* m = new Model::Model();
			m->WorldMatrix = Oyster::Math::Float4x4::identity;
			m->Visible = true;
			m->Animation[0].AnimationPlaying = nullptr;
			m->Animation[1].AnimationPlaying = nullptr;
			m->IgnoreDepth = false;
			m->Tint = Math::Float3(1);
			m->GlowTint = Math::Float3(1);
			m->Instanced = true;
			m->info = (Model::ModelInfo*)Core::loader.LoadResource((Core::modelPath + filename).c_str(),Oyster::Graphics::Loading::LoadDAN, Oyster::Graphics::Loading::UnloadDAN);

			Model::ModelInfo* mi = (Model::ModelInfo*)m->info;
			if(!mi || mi->Vertices->GetBufferPointer() == NULL)
			{
				delete m;
				Core::loader.ReleaseResource(mi);
				delete mi;
				return NULL;
			}
			
			if(!m->info->Animated)
			{
				if(Core::loader.GetResourceCount(m->info) == 1)
				{
					Render::Resources::RenderData[m->info] = new Render::Resources::ModelDataWrapper();
					Render::Resources::NoDepthData[m->info] = new Render::Resources::ModelDataWrapper();
				}
				else
				{
					Render::Resources::RenderData[m->info]->Models++;
					Render::Resources::NoDepthData[m->info]->Models++;
				}
			}

			return m;
		}

		void API::DeleteModel(Model::Model* model)
		{
			if(model==NULL)
				return;
			Model::ModelInfo* info = (Model::ModelInfo*)model->info;
			model->Animation[0].AnimationPlaying = nullptr;
			model->Animation[1].AnimationPlaying = nullptr;
			delete model;
			Core::loader.ReleaseResource(info);
		}

		void API::Clean()
		{
			Core::swapChain->SetFullscreenState(FALSE, NULL);
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

			for(auto i = Render::Resources::RenderData.begin(); i != Render::Resources::RenderData.end(); i++ )
			{
				SAFE_DELETE((*i).second->rid);
				SAFE_DELETE((*i).second);
			}

			for(auto i = Render::Resources::NoDepthData.begin(); i != Render::Resources::NoDepthData.end(); i++ )
			{
				SAFE_DELETE((*i).second->rid);
				SAFE_DELETE((*i).second);
			}

		}

		void API::AddLight(Definitions::Pointlight* light)
		{
			Lights.push_back(light);
		}

		void API::RemoveLight(Definitions::Pointlight* light)
		{
			for(int i=0;i<Lights.size();++i)
			{
				if(Lights[i]==light)
				{
					Lights[i] = Lights[Lights.size()-1];
					Lights.pop_back();
				}
			}
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
			Core::deviceContext->OMSetRenderTargets((UINT)Render::Resources::Gather::AnimatedPass.RTV.size(),&Render::Resources::Gather::AnimatedPass.RTV[0],Core::depthStencil);
			Core::deviceContext->RSSetState(wire);
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
#else
		API::State API::ReloadShaders()
		{
			return API::State::Sucsess;
		}

		void API::StartRenderWireFrame()
		{

		}

		void API::RenderDebugCube(Math::Matrix world)
		{

		}

		void API::RenderDebugSphere(Math::Matrix world)
		{

		}
#endif

		API::Option API::GetOption()
		{
			Option o;
			o.bytesUsed = Core::UsedMem/(1024*1024);
			o.modelPath = Core::modelPath;
			o.texturePath = Core::texturePath;
			o.resolution = Core::resolution;
			o.fullscreen = Core::fullscreen;
			o.ambientValue = Core::amb;
			o.globalGlowTint = Core::gGTint;
			o.globalTint = Core::gTint;
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

		float API::PlayAnimation( Model::Model* m, const std::wstring &name, bool looping )
		{
			if( m )
			{
				if( m->numOccupiedAnimationSlots < NumElementsOf(m->Animation) )
				{
					auto animBlueprint = m->info->Animations.find(name);
					if( animBlueprint != m->info->Animations.end() )
					{
						Model::AnimationData *animationSlot = &m->Animation[m->numOccupiedAnimationSlots]; // memory renaming for readability
						++m->numOccupiedAnimationSlots;

						animationSlot->AnimationPlaying	= &(animBlueprint->second);
						animationSlot->AnimationTime	= 0.0f;
						animationSlot->LoopAnimation	= looping;

						return (float)animationSlot->AnimationPlaying->duration;
					}
				}
			}
			return 0.0f;
		}

		void API::StopAnimation( Model::Model* m, const std::wstring &name )
		{
			if( m )
			{
				if( m->numOccupiedAnimationSlots < NumElementsOf(m->Animation) )
				{
					auto animBlueprint = m->info->Animations.find(name);
					if( animBlueprint != m->info->Animations.end() )
					{
						for( int i = 0; i < NumElementsOf(m->Animation); ++i )
						{
							if( m->Animation[i].AnimationPlaying == &animBlueprint->second )
							{
								--m->numOccupiedAnimationSlots;
								if( i < m->numOccupiedAnimationSlots )
								{
									m->Animation[i] = m->Animation[m->numOccupiedAnimationSlots];
								}
							}
						}
					}
				}
			}
		}

		void API::StopAllAnimations( Model::Model* m )
		{
			if( m )
			{
				m->numOccupiedAnimationSlots = 0;
			}
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