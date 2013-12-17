#ifndef DEFINES
#define DEFINES

struct PointLight
{
	float3 Pos;
	float Radius;
	float3 Color;
	float Bright;
};

struct DiffSpec
{
	float3 Diffuse;
	float3 Specular;
};

cbuffer LightConstants : register(b0)
{
	float4x4 InvProj;
	float2 Pixels;
	int Lights;
	float Pad;
}

Texture2D DiffuseGlow : register(t0);
Texture2D NormalSpec : register(t1);
Texture2D DepthTexture : register(t2);


StructuredBuffer<PointLight> Points : register(t3);

RWTexture2D<float4> Output : register(u0);

#endif