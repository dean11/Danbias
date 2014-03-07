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

			struct Model
			{
				ModelInfo* info;
				Oyster::Math::Float4x4 WorldMatrix;
				Oyster::Math::Float3 Tint;
				Oyster::Math::Float3 GlowTint;
				bool Visible;
				bool Instanced;
				bool IgnoreDepth;
				AnimationData Animation[4];
				int numOccupiedAnimationSlots;

				Model() { this->numOccupiedAnimationSlots = 0; }
				~Model() { }
			};
		}
	};
};

#endif