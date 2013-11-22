#pragma once
#ifndef Mesh_h
#define Mesh_h

#include "ModelInfo.h"


namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			struct Model
			{
				
				typedef unsigned long long ModelData;
				ModelData info;
				//ModelInfo* info;
				void* data;
				int size;
				bool Visible;
			};
		}
		
	};
};

#endif