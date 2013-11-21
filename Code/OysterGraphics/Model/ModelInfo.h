#pragma once
#ifndef MODELINFO_h
#define MODELINFO_h

//#include "../Engine.h"


#include "..\Core\CoreIncludes.h"
#include "..\Core\Buffer.h"
//#include "OysterMath.h"
//#include "ICollideable.h"

//using namespace Oyster::Math;

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			struct ModelInfo
			{
				std::vector<ID3D11ShaderResourceView*> Material;
				Buffer Vertices,Indecies;
				bool Indexed;
				int VertexCount;
			};	
		}
	}
};

#endif