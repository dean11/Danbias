#include "GFXAPI.h"
#include "../Core/Core.h"
#include "../Render/Resources/Debug.h"
#include "../Render/Resources/Deffered.h"
#include "../Render/Rendering/Render.h"
#include "../FileLoader/ObjReader.h"
//#include "../../Misc/Resource/OysterResource.h"
#include "../../Misc/Resource/ResourceManager.h"
#include "../FileLoader/GeneralLoader.h"
#include "../Model/ModelInfo.h"
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
		}

		API::State API::Init(HWND Window, bool MSAA_Quality, bool Fullscreen, Math::Float2 resulotion)
		{
			Core::resolution = resulotion;

			if(Core::Init::FullInit(Window, MSAA_Quality, Fullscreen) == Core::Init::Fail)
			{
				return API::Fail;
			}
			Render::Resources::Deffered::Init();

			Render::Preparations::Basic::SetViewPort();
			Render::Rendering::Basic::cube = API::CreateModel(L"box.dan");
			Render::Rendering::Basic::cube2 = API::CreateModel(L"box2.dan");
			return API::Sucsess;
		}

		void API::SetProjection(Math::Float4x4& projection)
		{
			Projection = projection;
		}

		void API::SetView(Math::Float4x4& view)
		{
			View = view;
		}

		void API::NewFrame()
		{
			if(Lights.size())
			{
				Render::Rendering::Basic::NewFrame(View, Projection, &Lights[0], (int)Lights.size());
			}
			else
			{
				Render::Rendering::Basic::NewFrame(View, Projection, NULL, 0);
			}
		}

		void API::RenderScene(Model::Model models[], int count)
		{
			Render::Rendering::Basic::RenderScene(models,count, View, Projection);
		}

		void API::RenderModel(Model::Model& m)
		{
			Render::Rendering::Basic::RenderScene(&m,1, View, Projection);
		}

		void API::EndFrame()
		{
			Render::Rendering::Basic::EndFrame();
		}

		API::State API::SetOptions(API::Option option)
		{
			Core::modelPath = option.modelPath;
			Core::texturePath = option.texturePath;
			return API::Sucsess;
		}

		//returns null for invalid filenames
		Model::Model* API::CreateModel(std::wstring filename)
		{
			Model::Model* m = new Model::Model();
			m->WorldMatrix = Oyster::Math::Float4x4::identity;
			m->Visible = true;
			m->AnimationPlaying = -1;
			m->info = (Model::ModelInfo*)Core::loader.LoadResource((Core::modelPath + filename).c_str(),Oyster::Graphics::Loading::LoadDAN, Oyster::Graphics::Loading::UnloadDAN);

			Model::ModelInfo* mi = (Model::ModelInfo*)m->info;
			if(!mi || mi->Vertices->GetBufferPointer() == NULL)
			{
				delete m;
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
			SAFE_DELETE(Core::viewPort);
			Core::loader.Clean();
			Oyster::Graphics::Core::PipelineManager::Clean();
			Oyster::Graphics::Render::Resources::Deffered::Clean();

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
			Render::Resources::Deffered::InitShaders();
			return State::Sucsess;
		}
#endif
	}
}