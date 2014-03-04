#include "AnimHeader.hlsli"

VertexOut main( AnimVertexIn input )
{
	VertexOut output;

	Matrix boneTrans = AnimatedData[input.boneIndex.x]*input.boneWeight.x +
					   AnimatedData[input.boneIndex.y]*input.boneWeight.y +
					   AnimatedData[input.boneIndex.z]*input.boneWeight.z +
					   AnimatedData[input.boneIndex.w]*input.boneWeight.w;

	input.pos = mul(boneTrans,float4(input.pos,1)).xyz * Animated + input.pos * int(1-Animated);
	
	input.normal = mul(boneTrans,float4(input.normal,0)).xyz * Animated + input.normal * int(1-Animated);

	output.pos = mul(WVP, float4(input.pos,1));
	output.ViewPos = mul(WV, float4(input.pos,1));
	output.normal = mul(WV, float4(input.normal,0)).xyz;
	output.UV = input.UV;
	return output;
}