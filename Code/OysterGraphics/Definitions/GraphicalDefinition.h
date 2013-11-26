#include "..\OysterGraphics\Core\CoreIncludes.h"
#include "OysterMath.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Definitions
		{
			struct ObjVertex
			{
				Oyster::Math::Float3 pos;
				Oyster::Math::Float2 uv;
				Oyster::Math::Float3 normal;
			};

			struct VP
			{
				Oyster::Math::Matrix V;
				Oyster::Math::Matrix P;
			};

			struct FinalVertex
			{
				Oyster::Math::Float3 pos;
				Oyster::Math::Float2 uv;
				Oyster::Math::Float3 normal;
				Oyster::Math::Float3 tangent;
				Oyster::Math::Float3 biTangent;
				Oyster::Math::Float4 boneIndex;
				Oyster::Math::Float4 boneWeights;
			};

		}
	}
}