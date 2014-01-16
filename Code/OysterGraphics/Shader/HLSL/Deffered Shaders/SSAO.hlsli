#include "Defines.hlsli"
#include "PosManipulation.hlsli"

static float Radius =5;

float GetSSAO(float3 pos, float2 uv, int2 texCoord2, uint2 rndID)
{
	
	float occlusion = 0.0f;
	//create sample coordinate system
	float4 rnd = float4( SSAORand[(rndID.x + rndID.y) % SSAORand.Length.x].xyz, 0.0f );
	rnd = normalize(rnd);
	float3 normal = NormalSpec[uv].xyz;
	float4 tangent = float4( normalize(rnd.xyz - (normal * dot(rnd.xyz, normal))), 0.0f );
	float4 biTangent = float4( cross(tangent.xyz, normal), 0.0f );

	float4x4 tbn = float4x4(tangent, biTangent, float4(normal,0), float4(pos*Radius,1));

	for( uint i = 0; i < SSAOKernel.Length.x; ++i )
	{
		//take sample from localspace to viewspace
		float4 sampled = mul(tbn, float4(SSAOKernel[i].xyz,1));
		//project sample to get uv.xy
		float4 ProjOffset = sampled;
		ProjOffset = mul(Proj, ProjOffset);
		float4 offset = ProjOffset;
		float2 UV = offset.xy;
		offset /= offset.w;
		offset.xyz = offset.xyz * 0.5f + 0.5f;
		//extra invert y axis, DX11
		offset.y = 1.0f - offset.y;
		
		// get depth from that point in screenspace
		uint2 texCoord;
		texCoord = (uint2)(offset.xy * Pixels);
		float3 ViewPos = ToVpos(texCoord, UV);

		float sampleDepth = ViewPos.z;

		//compare to depth from sample
		float rangeCheck = (abs(pos.z - sampleDepth) < Radius) ? 1.0f : 0.0f;
		occlusion += (sampleDepth >= sampled.z ? 1.0f : 0.0f) * rangeCheck;
	}
	occlusion /= (float)(SSAOKernel.Length.x);
	occlusion = 1.0f - occlusion;

	return occlusion;
}

