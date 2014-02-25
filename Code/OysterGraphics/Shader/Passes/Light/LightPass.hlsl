#include "Defines.hlsli"
#include "LightCalc.hlsli"
#include "PosManipulation.hlsli"
#include "SSAO.hlsli"
//todo
//LightCulling
//Calc Diff + Spec	Done
//Calc Ambience		Done
//Write Glow

#define EXPAND 1024.0f
#define SHRINK 1.0f/EXPAND
#define UINT_MAX	0xFFFFFFFF
#define FLOAT_MAX	3.402823466e+38
#define BLOCKSIZE 16
#define NUMTHREADS BLOCKSIZE * BLOCKSIZE
#define MAXLIGHTS 100


// -- Shared Memory ------------------------------------------------- //

groupshared uint iMinDepth = UINT_MAX,
				 iMaxDepth = 0;
groupshared uint numVisiblePointLights = 0,
				 visiblePointlightIndex[MAXLIGHTS];

// ------------------------------------------------------------------ //

[numthreads(BLOCKSIZE, BLOCKSIZE, 1)]
void main( uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID,  uint3 Gid : SV_GroupID, uint GI : SV_GroupIndex )
{
	float2 UV = DTid.xy / Pixels;
	UV.x =	UV.x * 2 - 1;
	UV.y = 1 - 2 * UV.y;

	float3 posN = float3(UV, DepthTexture[DTid.xy].x);

	// store and load shared minDepth and maxDepth
	float minDepth = 0.0f, maxDepth = 0.0f,
		  depth = posN.z;
	{
		uint uidepth = (uint)( depth  * EXPAND);
		InterlockedMin( iMinDepth, uidepth );
		InterlockedMax( iMaxDepth, uidepth );

		GroupMemoryBarrierWithGroupSync();
		minDepth = (float)( iMinDepth ) * SHRINK;
		maxDepth = (float)( iMaxDepth ) * SHRINK;
	}


	// -- Switching to LightCulling ------------------------------------- //

	//define collision volume
	float2 size = BLOCKSIZE / Pixels;
	FrustrumPoints tile;
	tile.v0 = float3(size * Gid,minDepth);
	tile.v1 = float3(tile.v0.xy+size,maxDepth);
	tile.v2 = float3(tile.v1.xy, minDepth);
	tile.v3 = float3(tile.v0.x,tile.v1.y,minDepth);
	tile.v4 = float3(tile.v1.x, tile.v0.y, minDepth);
	tile.v5 = float3(tile.v0.xy, maxDepth);



	// culling the tile's near and far to minDepth & maxDepth ( with tolerance )

		
		uint numPass = (Lights + NUMTHREADS - 1) / NUMTHREADS;
		numPass = min( numPass, MAXLIGHTS / NUMTHREADS );

		for( uint passI = 0; passI < numPass; ++passI )
		{
			uint lightIndex = (passI * NUMTHREADS) + GI;
			lightIndex = min( lightIndex, Lights );

			if( lightIndex < Lights ) 
				if( intersects(tile, lightIndex) )
			{
				uint offset;
				InterlockedAdd( numVisiblePointLights, 1, offset );
				visiblePointlightIndex[offset] = lightIndex;
			}
		}

	GroupMemoryBarrierWithGroupSync();


	
	float3 ViewPos = ToVpos(DTid.xy, UV);
	DiffSpec Shaded;
	Shaded.Diffuse = float3(0,0,0);
	Shaded.Specular = float3(0,0,0);

	for(int i = 0; i < Lights; ++i)
	{
		DiffSpec light = LightCalc(Points[i], ViewPos, DTid.xy);
		Shaded.Diffuse += light.Diffuse;
		Shaded.Specular += light.Specular;
	}

	Diffuse[DTid.xy] = float4(Shaded.Diffuse * DiffuseGlow[DTid.xy].xyz,0);
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
		DepthBase = DepthTexture[DTid.xy + uint2(1,0)];
		DepthBase = DepthTexture[DTid.xy + uint2(0,1)];
		DepthBase = DepthTexture[DTid.xy + uint2(1,1)];
		DepthBase = DepthBase /4;
		Ambient[DTid.xy/2] = float4(DiffBase.xyz, AmbValue);
		Ambient[DTid.xy/2 + float2(Pixels.x/2, 0)] = GUI[DTid.xy];
		Ambient[DTid.xy/2 + float2(0, Pixels.y/2)] = float4(DiffBase.xyz * DiffBase.w ,DiffBase.w);
		Ambient[DTid.xy/2 + Pixels/2] = float4(NormalSpec[DTid.xy].xyz * float3(1,1,-1),1);
	}

}