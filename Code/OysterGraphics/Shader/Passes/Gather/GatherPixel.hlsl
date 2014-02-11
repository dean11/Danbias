#include "Header.hlsli"

PixelOut main(VertexOut input)
{
	PixelOut output;
	output.DiffuseGlow = Diffuse.Sample(S1, input.UV);
	output.NormalSpec = float4(normalize(input.normal), Normal.Sample(S1,input.UV).w);
	return output;
}