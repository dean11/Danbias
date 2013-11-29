#include "GBufferHeader.hlsli"

PixelOut main(VertexOut input)
{
	PixelOut output;
	output.DiffuseGlow = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.NormalSpec = float4(input.normal, 1.0f);
	return output;
}