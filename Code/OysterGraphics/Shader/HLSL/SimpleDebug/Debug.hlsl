cbuffer PerFrame : register(b0)
{
	matrix View;
	float4x4 Projection;
}

cbuffer PerModel : register(b1)
{
	matrix World;
}

struct VertexOut
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
	float4 Wpos : POSITION;
};

struct VertexIn
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
	float3 normal : NORMAL;
};
