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

			struct Animation;

			struct AnimationData
			{
				Animation* AnimationPlaying;
				float AnimationTime;
				bool LoopAnimation;
			};
			struct AnimationPlayer
			{
				AnimationData data;
				ModelInfo* info;
			};
			struct Model
			{
				ModelInfo* info;
				Oyster::Math::Float4x4 WorldMatrix;
				bool Visible;
				AnimationPlayer Animation;
			};
		}
		
	};
};

#endif