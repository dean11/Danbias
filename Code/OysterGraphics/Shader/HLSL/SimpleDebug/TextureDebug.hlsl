#include "Debug.hlsl"

Texture2D tex : register(t0);
SamplerState S1 : register(s0);

float4 main(VertexOut inp) : SV_TARGET0
{
	return tex.Sample(S1,inp.UV);
}