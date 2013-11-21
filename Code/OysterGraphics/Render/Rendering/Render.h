#pragma once

#include "..\..\Core\Core.h"
#include "..\Preparations\Preparations.h"
#include "..\..\Model\Model.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Rendering
			{
				class Basic
				{
				public:
					class Resources
					{
						static Core::ShaderManager::ShaderEffect se;

						static void Init();
					};


					static void NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4 Projection);
					static void RenderScene(Model* models, int count);
					static void EndFrame();
				};
			}
		}
	}
}