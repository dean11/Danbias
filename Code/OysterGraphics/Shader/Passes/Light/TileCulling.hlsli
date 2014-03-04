#include "Defines.hlsli"


void CullLights(uint3 Gid, uint GI, bool Valid, float depth)
{
	// store and load shared minDepth and maxDepth
	float minDepth = 0.0f, maxDepth = 0.0f;
	{
		if(Valid)
		{
			InterlockedMin( iMinDepth, asuint(depth) );
			InterlockedMax( iMaxDepth, asuint(depth) );	
		}

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
		[unroll]
		for(int i = 0; i < 6; ++i)
		{
			float d = dot(frustumPlanes[i], float4(pl.Pos, 1.0f));
			inFrustrum = inFrustrum && (d >= -pl.Radius);
		}

		[branch]
		if(inFrustrum)
		{
			uint offset;
			InterlockedAdd( numVisiblePointLights, 1, offset );
			visiblePointlightIndex[offset] = lightIndex;
		}
	}

	GroupMemoryBarrierWithGroupSync();
}