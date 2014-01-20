#pragma once
#ifndef MODELINFO_h
#define MODELINFO_h


#include "..\Core\Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Model
		{
			struct ModelInfo
			{
				std::vector<ID3D11ShaderResourceView*> Material;
				Core::Buffer *Vertices,*Indecies;
				bool Indexed;
				int VertexCount, IndexCount;
			};	
		}
	}
};

#endif