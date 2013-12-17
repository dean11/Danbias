#include "GBufferHeader.hlsli"

PixelOut main(VertexOut input)
{
	PixelOut output;
	output.DiffuseGlow = Diffuse.Sample(S1, input.UV);
	output.NormalSpec = float4(normalize(input.normal), 1.0f);
	return output;
}