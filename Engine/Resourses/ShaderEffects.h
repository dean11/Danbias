#pragma once

#include "../Engine.h"
#include "Buffers.h"

namespace Oyster
{
	namespace Resources
	{
		struct ShaderEffects
		{
			static Oyster::Shader::ShaderEffect BasicSprite;
			static Oyster::Shader::ShaderEffect Text2DEffect;
			static Oyster::Shader::ShaderEffect ModelEffect;

			static void Init();

			static D3D11_INPUT_ELEMENT_DESC SpriteVertexDesc[1];
			static D3D11_INPUT_ELEMENT_DESC Text2DDesc[3];
			static D3D11_INPUT_ELEMENT_DESC ModelDesc[3];
		};
	}
}