#include "Defines.hlsli"
#include "PosManipulation.hlsli"



float GetSSAO(float3 pos, float2 uv, int2 texCoord2, uint2 rndID)
{
	
	float occlusion = 0.0f;
	//create sample coordinate system
	float4 rnd = float4( SSAORand[int2(rndID.x % (SSAORand.Length.x),  rndID.y % (SSAORand.Length.y))].xyz, 0.0f );
	rnd = normalize(rnd);
	float3 normal = NormalSpec[texCoord2].xyz;
	float3 tangent = float3( normalize(rnd.xyz - (normal * dot(rnd.xyz, normal))));
	float3 biTangent = float3( cross(tangent.xyz, normal));

	float3x3 tbn = transpose(float3x3(tangent, biTangent, normal));

	for( uint i = 0; i < SSAOKernel.Length.x; ++i )
	{
		//int i = 0;
		//take sample from localspace to viewspace

		float3 sampled = mul(tbn, SSAOKernel[i].xyz);
		sampled = sampled * SSAORadius + pos;

		//project sample to get uv.xy
		float4 ProjOffset = float4(sampled,1);
		ProjOffset = mul(Proj, ProjOffset);
		float4 offset = ProjOffset;
		float2 UV = offset.xy;
		offset /= offset.w;
		offset.xyz = offset.xyz * 0.5f + 0.5f;
		//extra invert y axis, DX11
		offset.y = 1.0f - offset.y;
		
		// get depth from that point in screenspace
		uint2 texCoord;
		texCoord = (uint2)(offset.xy * Diffuse.Length.xy);
		float3 ViewPos = ToVpos(texCoord, UV);

		float sampleDepth = ViewPos.z;

		//compare to depth from sample
		float rangeCheck = (abs(pos.z - sampleDepth) < SSAORadius) ? 1.0f : 0.0f;
		occlusion += (sampleDepth <= sampled.z ? 1.0f : 0.0f) * rangeCheck;
		//occlusion += rangeCheck;

	}
	occlusion /= (float)(SSAOKernel.Length.x);
	occlusion = 1.0f - occlusion;

	return occlusion;
}

