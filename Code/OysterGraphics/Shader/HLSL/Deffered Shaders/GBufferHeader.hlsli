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
	//float4 ViewPos		: POSITION;
	float2 UV			: TEXCOORD;
	float3 normal		: NORMAL;
	//float3 tangent		: TANGENT;
	//float3 biTangent	: BITANGENT;
};

struct PixelOut
{
	float4 DiffuseGlow	: SV_TARGET0;
	float4 NormalSpec	: SV_TARGET1;
};

Texture2D Diffuse		: register(t0);
Texture2D Normal		: register(t1);

SamplerState S1 : register(s0);

cbuffer Animation : register(b0)
{
	float4x4 AnimatedData[100];
	float4x4 BindPoseData[100];
	float4x4 RotationData[100];
	int Animated;
	float3 Pad;
}

cbuffer PerModel : register(b1)
{
	matrix WV;
	matrix WVP;
}