#include "OysterMath.h"
#pragma once

namespace Oyster
{
	namespace Graphics
	{
		namespace Definitions
		{
			struct ObjVertex
			{
				Oyster::Math::Float3 pos;
				Oyster::Math::Float2 uv;
				Oyster::Math::Float3 normal;
			};

			struct PerModel
			{
				Math::Matrix WV;
				Math::Matrix WVP;
				int Animated;
				Math::Float3 Pad;
			};

			struct FinalVertex
			{
				Oyster::Math::Float3 pos;
				Oyster::Math::Float2 uv;
				Oyster::Math::Float3 normal;
				Oyster::Math::Float4 boneIndex;
				Oyster::Math::Float4 boneWeights;
			};

			struct LightConstants
			{
				Math::Float4x4 InvProj;
				Math::Float4x4 Proj;
				Math::Float2 Pixels;
				int Lights;
				float SSAORadius;
				Oyster::Math::Float4x4 View;
			};

			struct Pointlight
			{
				Math::Float3 Pos;
				float Radius;
				Math::Float3 Color;
				float Bright;
			};

			struct AnimationData
			{
				Math::Float4x4 AnimatedData[100];
			};

			struct GuiData
			{
				Math::Matrix Translation;
			};

			struct PostData
			{
				float Amb;
			};

			struct Text2D
			{
				float pos;
				int offset;
				float coff;
			};

			struct BlurrData
			{
				unsigned int StartX;
				unsigned int StartY;
				unsigned int StopX;
				unsigned int StopY;
				Math::Float4 BlurMask;
			};
		}
	}
}