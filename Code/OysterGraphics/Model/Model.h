#pragma once
#ifndef Mesh_h
#define Mesh_h
#include "OysterMath.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Model
		{
			struct Model
			{
				//! do not Edit, linked to render data
				void* info;
				Oyster::Math::Float4x4 WorldMatrix;
				bool Visible;
			};
		}
		
	};
};

#endif