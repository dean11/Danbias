#include "Header.hlsli"

float4 main(Pixel2DIn input) : SV_Target0
{
	return Material.Sample(LinearSampler,input.Uv) * Color;
}