struct Vertex2DIn
{
	float2 Pos : Position;
};

cbuffer EveryObject2D : register(c0)
{
	float4x4 Translation;
};

struct Pixel2DIn
{
	float4 Pos : SV_Position;
	float2 Uv : TEXCOORD;
};

Texture2D Material : register(t0);
SamplerState LinearSampler : register(s0);