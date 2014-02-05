#include "GBufferHeader.hlsli"

VertexOut main( VertexIn input )
{
	VertexOut output;

	float3 offsetX = input.pos - BindPoseData[input.boneIndex.x][3].xyz;
	float3 offsetY = input.pos - BindPoseData[input.boneIndex.y][3].xyz;
	float3 offsetZ = input.pos - BindPoseData[input.boneIndex.z][3].xyz;
	float3 offsetW = input.pos - BindPoseData[input.boneIndex.w][3].xyz;

	Matrix boneTrans = AnimatedData[input.boneIndex.x]*input.boneWeight.x +
					   AnimatedData[input.boneIndex.y]*input.boneWeight.y +
					   AnimatedData[input.boneIndex.z]*input.boneWeight.z +
					   AnimatedData[input.boneIndex.w]*input.boneWeight.w; 

	input.pos = mul(boneTrans,float4(input.pos,1)) * Animated + input.pos * int(1-Animated);
	
	input.normal = mul(boneTrans,input.normal) * Animated + input.normal * int(1-Animated);
	/*input.pos =
		(mul(BoneAnimation[input.boneIndex.x], input.pos) * input.boneWeight.x * Animated) +
		(mul(BoneAnimation[input.boneIndex.y], input.pos) * input.boneWeight.y * Animated) +
		(mul(BoneAnimation[input.boneIndex.z], input.pos) * input.boneWeight.z * Animated) +
		(mul(BoneAnimation[input.boneIndex.w], input.pos) * input.boneWeight.w * Animated) +
		input.pos * int(1-Animated);*/

	/*input.pos = mul(BoneAnimation[input.boneIndex.x], input.pos) * Animated + input.pos * int(1-Animated);*/

		//float4x4 m = matrix(float4(1,0,0,0),float4(0,1,0,0), float4(0,0,1,0), float4(0,0,0,1));
		//input.pos = mul(BoneAnimation[0], float4(input.pos,1));
		//input.pos = mul(m, float4(input.pos,1));
	output.pos = mul(WVP, float4(input.pos,1));
	output.normal = mul(WV, float4(input.normal,0)).xyz;
	output.UV = input.UV;
	return output;
}