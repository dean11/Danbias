#include "LightMaterialMethods.hlsl"

#define BLOCKSIZE	16
#define NUMTHREADS	256
#define UINT_MAX	0xFFFFFFFF
#define FLOAT_MAX	3.402823466e+38
#define SSAOSphereRadius 10.0f
#define SPECULARITY_GLOSS_MAX 40.0f

Texture2D<float4> mapMaterialDiffuse	: register( t0 );
Texture2D<float4> mapMaterialSpecular	: register( t1 );
Texture2D<float4> mapGlow				: register( t2 );
Texture2D<float4> mapPosV				: register( t3 );
Texture2D<float4> mapNormalV			: register( t4 );
Texture2D<float4> mapDepth				: register( t5 );

StructuredBuffer<Frustrum> tileBuffer			: register( t6 );
StructuredBuffer<PointLight> pointLightBuffer	: register( t7 );
Texture1D<float3> rand							: register( t8 );
Texture1D<float3> sphere						: register( t9 );

RWTexture2D<float4> outputDiffuseIllum	: register( u0 );
RWTexture2D<float4> outputSpecularIllum	: register( u1 );
RWTexture2D<float4> outputGlowMap	    : register( u2 );
RWTexture2D<float4> outputSSAOMap	    : register( u3 );

cbuffer LightData : register( c0 )
{
	float4x4 viewMatrix;	
	float4x4 projectionMatrix;
	uint3 numDispatches;
	uint reservedPadding;
};

// -- Shared Memory ------------------------------------------------- //

groupshared uint iMinDepth = UINT_MAX,
				 iMaxDepth = 0;
groupshared uint numVisiblePointLights = 0,
				 visiblePointlightIndex[1024];

// ------------------------------------------------------------------ //

[numthreads( BLOCKSIZE, BLOCKSIZE, 1 )]
void lightComputer( uint3 groupID			: SV_GroupID,
					uint3 groupThreadID		: SV_GroupThreadID,
					uint  groupIndex		: SV_GroupIndex,
					uint3 dispatchThreadID	: SV_DispatchThreadID )
{
	uint dispatchIndex = dispatchThreadID.x + numDispatches.x * ( dispatchThreadID.y + (numDispatches.y * dispatchThreadID.z) );
	float3 posV = mapPosV[dispatchThreadID.xy].xyz;

	// store and load shared minDepth and maxDepth
	float minDepth = 0.0f, maxDepth = 0.0f,
			depth = length( posV );
	{
		uint uidepth = (uint)( depth * 1024.0f );
		InterlockedMin( iMinDepth, uidepth );
		InterlockedMax( iMaxDepth, uidepth );

		GroupMemoryBarrierWithGroupSync();
		minDepth = (float)( iMinDepth ) * 0.0009765625f;
		maxDepth = (float)( iMaxDepth ) * 0.0009765625f;
	}
	
// -- Switching to LightCulling ------------------------------------- //
	{
		Frustrum tile = tileBuffer[dispatchIndex];
		// culling the tile's near and far to minDepth & maxDepth ( with tolerance )
		tile.nearPlane.phasing = -(minDepth * 0.85f);
		tile.farPlane.phasing  =  (maxDepth * 1.15f);

		uint numPointLights = pointLightBuffer.Length.x,
				numPass = (numPointLights + NUMTHREADS - 1) / NUMTHREADS;
		numPass = min( numPass, 1024 / NUMTHREADS );

		for( uint passI = 0; passI < numPass; ++passI )
		{
			uint lightIndex = (passI * NUMTHREADS) + groupIndex;
			lightIndex = min( lightIndex, numPointLights );

			if( lightIndex < numPointLights ) 
				if( intersects(tile, pointLightBuffer[lightIndex].pos) )
			{
				uint offset;
				InterlockedAdd( numVisiblePointLights, 1, offset );
				visiblePointlightIndex[offset] = lightIndex;
			}
		}
	}
	GroupMemoryBarrierWithGroupSync();

	uint2 maxDim = mapMaterialDiffuse.Length.xy;
	if( dispatchThreadID.x < maxDim.x && dispatchThreadID.y < maxDim.y )
	{
// -- Switching to per Pixel Light Accumulation --------------------- //
		float specularGloss = mapMaterialSpecular[dispatchThreadID.xy].w * SPECULARITY_GLOSS_MAX;
		float3 normalV = mapNormalV[dispatchThreadID.xy].xyz;

		float3 diffuseLight = 0,
				specularLight = 0;
		float4 v = float4( 0.0f, 0.0f, 0.0f, 0.0f );

		for( uint lightI = 0; lightI < numVisiblePointLights; ++lightI )
		{ // for each light that might touch this pixel
			uint lightIndex = visiblePointlightIndex[lightI];
			PointLight light = pointLightBuffer[lightIndex]; // should be preloaded into groupshared methinks

			v = mul( viewMatrix, float4(light.pos.center, 1.0f) );
			light.pos.center = v.xyz / v.w;

			v = mul(viewMatrix, float4(light.pos.radius, 0.0f, 0.0f, 0.0f) );
			light.pos.radius = length( v.xyz );

			float exposure = 1.0f;
		
			accumulateLight( diffuseLight, specularLight, light, exposure, specularGloss, posV, normalV );
		}

// -- Applying SSAO ---------------------------------------- Pär H. - //
		float occlusion = 0.0f;
		{ //create sample coordinate system
			float4 rnd = float4( rand[groupIndex % rand.Length.x].xyz, 0.0f );


			float4 tangent = float4( normalize(rnd.xyz - (normalV * dot(rnd.xyz, normalV))), 0.0f );
			float4 biTangent = float4( cross(tangent.xyz, normalV), 0.0f );

			float4x4 tbn = float4x4( biTangent,
								tangent,
								float4(normalV, 0.0f),
								float4(posV, 1.0f) );

			for( uint i = 0; i < sphere.Length.x; ++i )
			{
				//take sample from localspace to viewspace
				float4 sampled = mul( float4(sphere[i].xyz,1), tbn);

				//project sample to get uv.xy
				float4 offset = float4(sampled);
				offset = mul( offset, projectionMatrix );
				offset.xy /= offset.w;
				offset.xy = offset.xy * 0.5 + 0.5;
				offset.y = 1.0f - offset.y;

				// get depth from that point in viewspace
				uint2 texCoord;
				texCoord.x = (uint)(offset.x * (float)(mapMaterialDiffuse.Length.x));
				texCoord.y = (uint)(offset.y * (float)(mapMaterialDiffuse.Length.y));
				float sampleDepth = length(mapPosV[texCoord]);
				float rangeDepth = mapPosV[texCoord].z;

				//compare to depth from sample
				float rangeCheck = (abs(depth - sampleDepth) < SSAOSphereRadius) ? 1.0f : 0.0f;
				occlusion += (sampleDepth <= length(sampled) ? 1.0f : 0.0f) * rangeCheck;
			}
			occlusion /= (float)(sphere.Length.x);
			occlusion = 1.0f - occlusion;
		}

// -- Compile and write Light values to buffers --------------------- //

		diffuseLight.xyz *= mapMaterialDiffuse[dispatchThreadID.xy].xyz;
		specularLight.xyz *= mapMaterialSpecular[dispatchThreadID.xy].xyz;
		float4 glow = mapGlow[dispatchThreadID.xy];
		glow *= glow.w;
	
		//specularLight.xyz += mapGlow[dispatchThreadID.xy].xyz;

		outputDiffuseIllum[dispatchThreadID.xy] = float4( diffuseLight, 1.0f );
		outputSpecularIllum[dispatchThreadID.xy] = float4( specularLight.xyz, 1 );
		outputGlowMap[dispatchThreadID.xy] = float4( glow.xyz * glow.z, 1 );
		outputSSAOMap[dispatchThreadID.xy] = float4( occlusion, occlusion, occlusion, 1 );
	}
}
/* ------------------------------------------------------------------ //
	Needs to split into two passes here. As a major Dispatch sync is
	is warranted for crossDispatch datasharing.
// ------------------------------------------------------------------ */

Texture2D<float4> mapDiffuse        	: register( t0 );
Texture2D<float4> mapSpecular	        : register( t1 );
Texture2D<float4> Glow   				: register( t2 );
Texture2D<float4> mapSSAO				: register( t3 );

RWTexture2D<float4> outputBackBuffer	: register( u0 );

[numthreads( BLOCKSIZE, BLOCKSIZE, 1 )]
void composingComputer( uint3 groupID			: SV_GroupID,
						uint3 groupThreadID		: SV_GroupThreadID,
						uint  groupIndex		: SV_GroupIndex,
						uint3 dispatchThreadID	: SV_DispatchThreadID )
{
	uint2 maxDim = mapDiffuse.Length.xy;
	if( dispatchThreadID.x < maxDim.x && dispatchThreadID.y < maxDim.y )
	{
// -- Switching to light-material compiling ------------------------- //
		float3 diffuse = mapDiffuse[dispatchThreadID.xy].xyz;
		float3 specular = mapSpecular[dispatchThreadID.xy].xyz;
		float3 glow = Glow[dispatchThreadID.xy].xyz;
		float SSAO = mapSSAO[dispatchThreadID.xy].x;

		float3 I = diffuse * SSAO + specular * SSAO + glow * 0.5f;
		
		//outputBackBuffer[dispatchThreadID.xy] = float4( SSAO,SSAO,SSAO, 1.0f );
		//return;
		
		uint2 midScreen = mapDiffuse.Length.xy / 2;
		if( dispatchThreadID.x < midScreen.x )
		{
			if( dispatchThreadID.y < midScreen.y )
				outputBackBuffer[dispatchThreadID.xy] = float4( I, 1.0f );
			else
				outputBackBuffer[dispatchThreadID.xy] = float4( diffuse + specular, 1.0f );
		}
		else
		{
			if( dispatchThreadID.y < midScreen.y )
				outputBackBuffer[dispatchThreadID.xy] = float4( glow * 0.5f, 1.0f );
			else
				outputBackBuffer[dispatchThreadID.xy] = float4( SSAO, SSAO, SSAO, 1.0f );
		}/**/
	}
}