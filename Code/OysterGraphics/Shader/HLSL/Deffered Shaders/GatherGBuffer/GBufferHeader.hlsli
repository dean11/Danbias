struct VertexIn
{
	float3 pos			: POSITION;
	float2 UV			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 biTangent	: BITANGENT;
	float4 boneIndex	: BONEINDEX;
	float4 boneWeight	: BONEWEIGHT;
};

struct VertexOut
{
	float4 pos			: SV_POSITION;
	float4 ViewPos		: POSITION;
	float2 UV			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 biTangent	: BITANGENT;
};

struct PixelOut
{
	float4 DiffuseGlow	: SV_TARGET0;
	float4 NormalSpec	: SV_TARGET1;
};

Texture2D Diffuse		: register(t0);
Texture2D Specular		: register(t1);

cbuffer PerFrame : register(b0)
{
	matrix View;
	float4x4 Projection;
	matrix VP;
}

cbuffer PerModel : register(b1)
{
	matrix World;
}