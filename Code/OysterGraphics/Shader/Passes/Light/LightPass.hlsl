#include "Defines.hlsli"
#include "LightCalc.hlsli"
#include "PosManipulation.hlsli"
#include "SSAO.hlsli"
//todo
//LightCulling
//Calc Diff + Spec	Done
//Calc Ambience		Done
//Write Glow

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

// ------------------------------------------------------------------ //

[numthreads(BLOCKSIZE, BLOCKSIZE, 1)]
void main( uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID,  uint3 Gid : SV_GroupID, uint GI : SV_GroupIndex )
{
	float2 UV = DTid.xy / (float2)Diffuse.Length.xy;
	UV.x =	UV.x * 2 - 1;
	UV.y = 1 - 2 * UV.y;

	float3 posN = float3(UV, DepthTexture[DTid.xy].x);
	
	float3 ViewPos = ToVpos(DTid.xy, UV);

	if(GI==0)
	{
		numVisiblePointLights = 0;
		iMinDepth = 0x7F7FFFFF;
		iMaxDepth = 0;
	}
	GroupMemoryBarrierWithGroupSync();

	// store and load shared minDepth and maxDepth
	float minDepth = 0.0f, maxDepth = 0.0f;
	{
		InterlockedMin( iMinDepth, asuint(ViewPos.z) );
		InterlockedMax( iMaxDepth, asuint(ViewPos.z) );

		GroupMemoryBarrierWithGroupSync();
		minDepth = asfloat(iMinDepth);
		maxDepth = asfloat(iMaxDepth);
	}


	// -- Switching to LightCulling ------------------------------------- //

	//define collision volume
	float2 tilescale = float2(Diffuse.Length.xy) * rcp(float(2 * BLOCKSIZE));
	float2 tilebias = tilescale - float2(Gid.xy);

	// Now work out composite projection matrix
	// Relevant matrix columns for this tile frusta
	float4 c1 = float4(Proj._11 * tilescale.x, 0.0f, tilebias.x, 0.0f);
	float4 c2 = float4(0.0f, -Proj._22 * tilescale.y, tilebias.y, 0.0f);
	float4 c4 = float4(0.0f, 0.0f, 1.0f, 1.0f);

	// Derive frustum planes
	float4 frustumPlanes[6];
	// Sides
	frustumPlanes[0] = c4 - c1;
	frustumPlanes[1] = c4 + c1;
	frustumPlanes[2] = c4 - c2;
	frustumPlanes[3] = c4 + c2;
	// Near/far
	frustumPlanes[4] = float4(0.0f, 0.0f,  1.0f, -minDepth);
	frustumPlanes[5] = float4(0.0f, 0.0f, -1.0f,  maxDepth);

	// Normalize frustum planes (near/far already normalized)
	[unroll]
	for (uint i = 0; i < 4; ++i)
	{
		frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
	}

	// culling the tile's near and far to minDepth & maxDepth ( with tolerance )

		
	for(uint lightIndex = GI; lightIndex < Lights; lightIndex += NUMTHREADS)
	{
		PointLight pl = Points[lightIndex];
	
		bool inFrustrum = true;
		float d = dot(frustumPlanes[0], float4(pl.Pos, 1.0f));
		inFrustrum = inFrustrum && (d >= -pl.Radius);
		d = dot(frustumPlanes[1], float4(pl.Pos, 1.0f));
		inFrustrum = inFrustrum && (d >= -pl.Radius);
		d = dot(frustumPlanes[2], float4(pl.Pos, 1.0f));
		inFrustrum = inFrustrum && (d >= -pl.Radius);
		d = dot(frustumPlanes[3], float4(pl.Pos, 1.0f));
		inFrustrum = inFrustrum && (d >= -pl.Radius);
		d = dot(frustumPlanes[4], float4(pl.Pos, 1.0f));
		inFrustrum = inFrustrum && (d >= -pl.Radius);
		d = dot(frustumPlanes[5], float4(pl.Pos, 1.0f));
		inFrustrum = inFrustrum && (d >= -pl.Radius);

		[branch]
		if(inFrustrum)
		{
			uint offset;
			InterlockedAdd( numVisiblePointLights, 1, offset );
			visiblePointlightIndex[offset] = lightIndex;
		}
	}

	GroupMemoryBarrierWithGroupSync();


	
	DiffSpec Shaded;
	Shaded.Diffuse = float3(0,0,0);
	Shaded.Specular = float3(0,0,0);

	for(int i = 0; i < numVisiblePointLights; ++i)
	{
		DiffSpec light = LightCalc(Points[visiblePointlightIndex[i]], ViewPos, DTid.xy);
		Shaded.Diffuse += light.Diffuse;
		Shaded.Specular += light.Specular;
	}

	Diffuse[DTid.xy] = float4(Shaded.Diffuse * DiffuseGlow[DTid.xy].xyz,1);
	Specular[DTid.xy] = float4(Shaded.Specular, 0);

	
	if(DTid.x & 1 && DTid.y & 1 )
	{
		float AmbValue = GetSSAO(ViewPos, UV, DTid.xy, GTid.xy/2);
		float4 DiffBase = DiffuseGlow[DTid.xy];
		DiffBase += DiffuseGlow[DTid.xy + uint2(1,0)];
		DiffBase += DiffuseGlow[DTid.xy + uint2(0,1)];
		DiffBase += DiffuseGlow[DTid.xy + uint2(1,1)];
		DiffBase = DiffBase / 4;
		
		float4 DepthBase = DepthTexture[DTid.xy];
		DepthBase += DepthTexture[DTid.xy + uint2(1,0)];
		DepthBase += DepthTexture[DTid.xy + uint2(0,1)];
		DepthBase += DepthTexture[DTid.xy + uint2(1,1)];
		DepthBase = DepthBase /4;
		Ambient[DTid.xy/2] = float4(DiffBase.xyz , AmbValue);
		//Ambient[DTid.xy/2] = float4(DiffBase.xyz, 1);
		Ambient[DTid.xy/2 + float2(Diffuse.Length.x/2, 0)] = GUI[DTid.xy];
		Ambient[DTid.xy/2 + float2(0, Diffuse.Length.y/2)] = float4(DiffBase.xyz * DiffBase.w ,DiffBase.w);
		Ambient[DTid.xy/2 + Diffuse.Length.xy/2] = float4(numVisiblePointLights * (1.0f/Lights), 0, 0 ,1);
	}

}