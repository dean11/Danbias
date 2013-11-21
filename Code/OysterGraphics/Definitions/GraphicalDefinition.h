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
		}
	}
}