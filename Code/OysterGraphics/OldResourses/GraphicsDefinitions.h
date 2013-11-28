#pragma once


#include "..\EngineIncludes.h"

namespace Oyster
{
	namespace Resources
	{

		namespace BufferDefinitions
		{
			struct LightStructureBuffer
			{
				::Oyster::Math::Float4x4 viewMatrix, projectionMatrix;
				::LinearAlgebra::Vector3<unsigned int> numDispatches;
				unsigned int reservedPadding;
			};

			struct ScreenTileFrustrum
			{
				::Oyster::Math::Float rawElement[6 * 4];
			};

			class PointLightDescription
			{
			public:
				struct{ ::Oyster::Math::Float3 center; ::Oyster::Math::Float radius; } pos;
				::Oyster::Math::Float3 color;
				::Oyster::Math::Float intensty;
			};
		};
	}
}