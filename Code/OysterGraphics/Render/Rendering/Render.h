#pragma once
#include "..\..\Definitions\GraphicalDefinition.h"
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

					static void NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection, Definitions::Pointlight* Lights, int numLights);
					static void RenderScene(Model::Model* models, int count, Math::Matrix View, Math::Matrix Projection);
					static void EndFrame();
				};
			}
		}
	}
}