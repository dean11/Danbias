#pragma once

#include "../Core/Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			class Gui
			{
			public:
				static void Begin2DRender();
				static void Render(ID3D11ShaderResourceView* tex, Math::Float3 pos, Math::Float2 size, Math::Float3 tint = Math::Float3(1,1,1));
				static void Begin2DTextRender();
				static void RenderText(std::wstring text, Math::Float3 pos, Math::Float2 size, float FontSize, Math::Float3 tint = Math::Float3(1,1,1));
			};
		}
	}
}