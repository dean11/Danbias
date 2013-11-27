#include "GBufferHeader.hlsli"

PixelOut main(VertexOut input)
{
	PixelOut output;
	output.DiffuseGlow = Diffuse.Sample(Sampler, input.UV);
	output.NormalSpec = float4(input.normal, 1.0f);
	return output;
}