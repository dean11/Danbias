#pragma once
#ifndef MODELINFO_h
#define MODELINFO_h


#include "..\Core\CoreIncludes.h"
#include "..\Core\Buffer.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			struct ModelInfo
			{
				std::vector<ID3D11ShaderResourceView*> Material;
				Buffer *Vertices,*Indecies;
				bool Indexed;
				int VertexCount;
			};	
		}
	}
};

#endif