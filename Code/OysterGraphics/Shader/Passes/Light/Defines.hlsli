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
	float3 Ambient;
};

cbuffer LightConstants					: register(b0)
{
	float4x4 InvProj;
	float4x4 Proj;
	float AmbFactor;
	int Lights;
	float SSAORadius;
	float pad;
	float4x4 View;
}

struct FrustrumPoints
{
	float3 NUL;
	float3 NUR;
	float3 NDL;
	float3 NDR;

	float3 FUL;
	float3 FUR;
	float3 FDL;
	float3 FDR;
};

// -- Indata -------------------------------------------------//
Texture2D DiffuseGlow					: register(t0);
Texture2D NormalSpec					: register(t1);
Texture2D GUI							: register(t2);
Texture2D DepthTexture					: register(t3);

StructuredBuffer<PointLight> Points		: register(t4);

Texture1D SSAOKernel					: register(t5);
Texture2D SSAORand						: register(t6);

//SkyBox
Texture2D Up							: register(t7);
Texture2D Down							: register(t8);
Texture2D Left							: register(t9);
Texture2D Right							: register(t10);
Texture2D Front							: register(t11);
Texture2D Back							: register(t12);

Texture2D NoDepth						: register(t13);

// -- Output ----------------------------------------------------//

RWTexture2D<float4> Diffuse				: register(u0);
RWTexture2D<float4> Specular			: register(u1);
RWTexture2D<float4> Ambient				: register(u2);


// -- Defines -----------------------------------------------------//

#define UINT_MAX	0xFFFFFFFF
#define FLOAT_MAX	3.402823466e+38
#define BLOCKSIZE 16
#define NUMTHREADS BLOCKSIZE * BLOCKSIZE
#define MAXLIGHTS 1024
#define TEXTURESPREAD 1/255

// -- Shared Memory ------------------------------------------------- //

groupshared uint iMinDepth, iMaxDepth;
groupshared uint numVisiblePointLights,
				 visiblePointlightIndex[MAXLIGHTS];

#endif