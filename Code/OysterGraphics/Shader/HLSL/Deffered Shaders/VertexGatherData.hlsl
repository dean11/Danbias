#include "GBufferHeader.hlsli"

VertexOut main( VertexIn input )
{
	VertexOut output;
	input.pos = mul(BoneAnimation[input.boneIndex.x], input.pos) * Animated + input.pos * int(1-Animated);
		//float4x4 m = matrix(float4(1,0,0,0),float4(0,1,0,0), float4(0,0,1,0), float4(0,0,0,1));
		//input.pos = mul(BoneAnimation[0], float4(input.pos,1));
		//input.pos = mul(m, float4(input.pos,1));
	output.pos = mul(WVP, float4(input.pos,1));
	output.normal = mul(WV, float4(input.normal,0)).xyz;
	output.UV = input.UV;
	return output;
}