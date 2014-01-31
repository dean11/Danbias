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
				Math::Float4x4 Transform;
				int Parent;
			};
			struct Frame
			{
				Bone bone;
				double time;
			};
			struct Animation
			{
				std::wstring name;
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
				Animation* Animations;
			};	
		}
	}
};

#endif