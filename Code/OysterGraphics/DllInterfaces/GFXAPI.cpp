#include "GFXAPI.h"
#include "../Core/Core.h"
#include "../Render/Resources/Resources.h"
#include "../Render/Rendering/Render.h"

namespace Oyster
{
	namespace Graphics
	{
		API::State API::Init(HWND Window, bool MSAA_Quality, bool Fullscreen)
		{
			if(Core::Init::FullInit(Window, MSAA_Quality, Fullscreen) == Core::Init::Fail)
			{
				return API::Fail;
			}
			if(Render::Resources::Init() == Core::Init::Fail)
			{
				return API::Fail;
			}
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

	}
}