#include "GFXAPI.h"
#include "../Core/Core.h"
#include "../Render/Resources/Debug.h"
#include "../Render/Resources/Deffered.h"
#include "../Render/Rendering/Render.h"
#include "../FileLoader/ObjReader.h"
#include "../../Misc/Resource/OysterResource.h"
#include "../FileLoader/GeneralLoader.h"

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
#ifdef _DEBUG
			if(Render::Resources::Debug::Init() == Core::Init::Fail)
			{
				return API::Fail;
			}
#endif
			Render::Resources::Deffered::Init();

			Render::Preparations::Basic::SetViewPort();
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
				Render::Rendering::Basic::NewFrame(View, Projection, &Lights[0], Lights.size());
			}
			else
			{
				Render::Rendering::Basic::NewFrame(View, Projection, NULL, 0);
			}
		}

		void API::RenderScene(Model::Model models[], int count)
		{
			Render::Rendering::Basic::RenderScene(models,count);
		}

		void API::RenderModel(Model::Model& m)
		{
			Render::Rendering::Basic::RenderScene(&m,1);
		}

		void API::EndFrame()
		{
			Render::Rendering::Basic::EndFrame();
		}

		API::State API::SetOptions(API::Option option)
		{
			return API::Sucsess;
		}

		Model::Model* API::CreateModel(std::wstring filename)
		{
			Model::Model* m = new Model::Model();
			m->WorldMatrix = Oyster::Math::Float4x4::identity;
			m->Visible = true;

			m->info = Oyster::Resource::OysterResource::LoadResource(filename.c_str(),Oyster::Graphics::Loading::LoadOBJ);

			return m;
		}

		void API::DeleteModel(Model::Model* model)
		{
			Model::ModelInfo* info = (Model::ModelInfo*)model->info;
			delete model;
			Oyster::Resource::OysterResource::ReleaseResource((Oyster::Resource::OHRESOURCE)info);
		}

		void API::Clean()
		{
			SAFE_DELETE(Core::viewPort);
			Oyster::Resource::OysterResource::Clean();
			Oyster::Graphics::Core::PipelineManager::Clean();

#ifdef _DEBUG
			Oyster::Graphics::Render::Resources::Debug::Clean();
#endif
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
	}
}