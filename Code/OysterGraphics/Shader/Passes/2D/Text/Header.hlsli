#include "../Header.hlsli"
cbuffer TextPerObject : register(b0)
{
	float4x4 gWorld;
};

Texture2D g_tex1 : register(t0);

struct Text2DIn
{
	float Pos : Position;
	int off : Offset;
	float coff : CharOffset;
};

struct TEXT_VS_OUT
{
	float4 pos    : SV_POSITION;
	float2 texCoord : TEXCOORD;
};