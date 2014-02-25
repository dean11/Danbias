struct InstVertexIn
{
	float3 pos			: POSITION;
	float2 UV			: TEXCOORD;
	float3 normal		: NORMAL;
	float4 boneIndex	: BONEINDEX;
	float4 boneWeight	: BONEWEIGHT;
	//instance Data
	//WV MATRIX
	float4 wv1			: WV0;
	float4 wv2			: WV1;
	float4 wv3			: WV2;
	float4 wv4			: WV3;

	//WVP MATRIX
	float4 wvp1			: WVP0;
	float4 wvp2			: WVP1;
	float4 wvp3			: WVP2;
	float4 wvp4			: WVP3;

	float3 Tint			: TINT;
	float3 GlowTint		: GTINT;
};

struct VertexOut
{
	float4 pos			: SV_POSITION;
	float4 ViewPos		: POSITION;
	float2 UV			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tint			: TINT;
	float3 gtint		: GTINT;
};

struct PixelOut
{
	float4 DiffuseGlow	: SV_TARGET0;
	float4 NormalSpec	: SV_TARGET1;
};

Texture2D Diffuse		: register(t0);
Texture2D Normal		: register(t1);

SamplerState S1 : register(s0);