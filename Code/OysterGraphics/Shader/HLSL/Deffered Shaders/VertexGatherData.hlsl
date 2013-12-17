#include "GBufferHeader.hlsli"

VertexOut main( VertexIn input )
{
	VertexOut output;
	float4 WorldPos = mul(World, float4(input.pos,1));
	float4 ViewPos = mul(View, WorldPos);
	output.pos = mul(Projection, ViewPos);
	float4 WorldNor = mul(World, float4(input.normal,0));
	float4 ViewNor = mul(View, WorldNor);
	output.normal = ViewNor;
	output.normal.z *= -1;

	output.UV = input.UV;
	//output.normal = ViewNor;
	return output;
}