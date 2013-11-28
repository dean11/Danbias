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

					static void NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection);
					static void RenderScene(Model::Model* models, int count);
					static void EndFrame();
				};
			}
		}
	}
}