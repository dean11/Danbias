#ifndef DEFINES
#define DEFINES

struct PointLight
{
	float4 PosRadius;
	float4 ColorBright;
};

struct DiffSpec
{
	float3 Diffuse;
	float3 Specular;
};

cbuffer PointLights : register(b0)
{
	PointLight pl;
}

cbuffer LightConstants : register(b1)
{
	float4x4 InvProj;
	int2 Pixels;
}

Texture2D DiffuseGlow : register(t0);
Texture2D NormalSpec : register(t1);
Texture2D DepthTexture : register(t2);

#endif