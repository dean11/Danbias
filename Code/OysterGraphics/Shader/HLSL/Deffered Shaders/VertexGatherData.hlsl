#include "GBufferHeader.hlsli"

VertexOut main( VertexIn input )
{
	VertexOut output;
	output.pos = mul(WVP, float4(input.pos,1));
	output.normal = mul(WV, float4(input.normal,0));
	output.UV = input.UV;
	return output;
}