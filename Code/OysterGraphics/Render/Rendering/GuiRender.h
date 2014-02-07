#pragma once

#include "../../Core/Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Rendering
			{
				class Gui
				{
				public:
					static void BeginRender();
					static void Render(ID3D11Texture2D* tex, Math::Float2 pos, Math::Float2 size);
				};
			}
		}
	}
}