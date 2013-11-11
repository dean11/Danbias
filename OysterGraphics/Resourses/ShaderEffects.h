#pragma once

#include "../Engine.h"
#include "Buffers.h"

namespace Oyster
{
	namespace Resources
	{
		struct ShaderEffects
		{
			static Oyster::Core::ShaderManager::ShaderEffect BasicSprite;
			static Oyster::Core::ShaderManager::ShaderEffect Text2DEffect;
			static Oyster::Core::ShaderManager::ShaderEffect ModelEffect;

			static void Init();

			static D3D11_INPUT_ELEMENT_DESC SpriteVertexDesc[1];
			static D3D11_INPUT_ELEMENT_DESC Text2DDesc[3];
			static D3D11_INPUT_ELEMENT_DESC ModelDesc[3];
		};
	}
}