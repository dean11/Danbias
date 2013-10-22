#pragma once
#ifndef Mesh_h
#define Mesh_h

//#include "../Engine.h"


//#include "..\Core\CoreIncludes.h"
//#include "..\Core\Buffer.h"
#include "OysterMath.h"
//#include "ICollideable.h"
#include "ModelInfo.h"

using namespace Oyster::Math;

namespace Oyster
{
	namespace Render
	{

		struct Model
		{
			ModelInfo* info;
			Float4x4 *World;
			bool Visible;
		};

		
	};
};

#endif