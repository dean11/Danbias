#include "GFXAPI.h"
#include "../Core/Core.h"
#include "../Render/Resources/Resources.h"
#include "../Render/Rendering/Render.h"
#include "../FileLoader/ObjReader.h"

namespace Oyster
{
	namespace Graphics
	{
		API::State API::Init(HWND Window, bool MSAA_Quality, bool Fullscreen, Math::Float2 resulotion)
		{
			Core::resolution = resulotion;

			if(Core::Init::FullInit(Window, MSAA_Quality, Fullscreen) == Core::Init::Fail)
			{
				return API::Fail;
			}
			if(Render::Resources::Init() == Core::Init::Fail)
			{
				return API::Fail;
			}

			Render::Preparations::Basic::SetViewPort();
			return API::Sucsess;
		}

		void API::NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection)
		{
			Render::Rendering::Basic::NewFrame(View, Projection);
		}

		void API::RenderScene(Model::Model* models, int count)
		{
			Render::Rendering::Basic::RenderScene(models,count);
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

			OBJReader or;
			or.readOBJFile(filename);
			m->info = or.toModel();

			return m;
		}

		void API::DeleteModel(Model::Model* model)
		{
			delete model;
		}
	}
}