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

Texture2D DiffuseGlow					: register(t0);
Texture2D NormalSpec					: register(t1);
Texture2D DepthTexture					: register(t2);

StructuredBuffer<PointLight> Points		: register(t3);

Texture1D SSAOKernel					: register(t4);
Texture1D SSAORand						: register(t5);

RWTexture2D<float4> Diffuse				: register(u0);
RWTexture2D<float4> Specular			: register(u1);
RWTexture2D<float> Ambient				: register(u2);

#endif