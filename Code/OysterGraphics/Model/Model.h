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
				
				void* const info;
				//ModelInfo* info;
				void* data;
				int size;
				bool Visible;
			};
		}
		
	};
};

#endif