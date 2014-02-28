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
			struct Bone
			{
				Math::Matrix Relative;
				Math::Matrix Absolute;
				int Parent;
			};

			struct Frame
			{
				Bone bone;
				double time;
			};

			struct Animation
			{
				int Bones;
				int* Frames; //! Bone as index
				Frame** Keyframes; //! @brief [Bone][Frame]
				double duration;
			};

			struct ModelInfo
			{
				std::vector<ID3D11ShaderResourceView*> Material;
				Core::Buffer *Vertices,*Indecies;
				bool Indexed, Animated;
				int VertexCount, IndexCount, BoneCount, AnimationCount;
				Bone* bones;
				std::map<std::wstring,Animation> Animations;
			};	
		}
	}
};

#endif