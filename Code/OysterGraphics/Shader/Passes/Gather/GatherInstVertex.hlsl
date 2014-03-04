#include "InstHeader.hlsli"

VertexOut main( InstVertexIn input )
{
	VertexOut output;

	matrix WV = transpose(matrix(input.wv1, input.wv2, input.wv3, input.wv4));
	matrix WVP = transpose(matrix(input.wvp1, input.wvp2, input.wvp3, input.wvp4));

	output.pos = mul(WVP, float4(input.pos,1));
	output.ViewPos = mul(WV, float4(input.pos,1));
	output.normal = mul(WV, float4(input.normal,0)).xyz;
	output.UV = input.UV;
	output.tint = input.Tint;
	output.gtint = input.GlowTint;

	return output;
}