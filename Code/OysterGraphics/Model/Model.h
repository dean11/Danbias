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
			struct ModelInfo;
			struct Model
			{
				ModelInfo* info;
				Oyster::Math::Float4x4 WorldMatrix;
				bool Visible, LoopAnimation;
				int AnimationPlaying;
				float AnimationTime;
			};
		}
		
	};
};

#endif