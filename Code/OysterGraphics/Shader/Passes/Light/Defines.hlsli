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

cbuffer LightConstants					: register(b0)
{
	float4x4 InvProj;
	float4x4 Proj;
	float2 Pixels;
	int Lights;
	float SSAORadius;
	float4x4 View;
}

struct FrustrumPoints
{
	float3 v0;
	float3 v1;
	float3 v2;
	float3 v3;
	float3 v4;
	float3 v5;
};

Texture2D DiffuseGlow					: register(t0);
Texture2D NormalSpec					: register(t1);
Texture2D GUI							: register(t2);
Texture2D DepthTexture					: register(t3);

StructuredBuffer<PointLight> Points		: register(t4);

Texture1D SSAOKernel					: register(t5);
Texture2D SSAORand						: register(t6);

RWTexture2D<float4> Diffuse				: register(u0);
RWTexture2D<float4> Specular			: register(u1);
RWTexture2D<float4> Ambient				: register(u2);

#endif