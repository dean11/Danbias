#pragma once

namespace Oyster
{
	namespace Graphics
	{
		namespace Definitions
		{
			struct Vertex
			{
				float data[16];
				/*
				Oyster::Math::Float3 pos;
				Oyster::Math::Float2 uv;
				Oyster::Math::Float3 normal;
				Oyster::Math::Float4 boneIndex;
				Oyster::Math::Float4 boneWeights;*/
			};

			struct FinalVertex
			{
				float pos[3];
			};

		}
	}
}

typedef Oyster::Graphics::Definitions::Vertex Vertex;
typedef Oyster::Graphics::Definitions::FinalVertex FinalVertex;

struct ModelInfo
{
	unsigned int numVertex;
	unsigned int numIndex;

	FinalVertex* vertices;
	unsigned int* indices;
};