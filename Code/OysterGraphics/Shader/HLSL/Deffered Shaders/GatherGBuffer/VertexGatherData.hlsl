#include "GBufferHeader.hlsli"

VertexOut main( VertexIn input )
{
	VertexOut output;
	matrix WV = mul(View, World);
	output.ViewPos = mul(WV, float4(input.pos,1));
	output.pos = mul(Projection, output.ViewPos);
	output.UV = input.UV;
	output.normal = float4(input.normal, 0);
	return output;
}