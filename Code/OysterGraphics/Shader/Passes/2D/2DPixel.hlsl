#include "Header.hlsli"

float4 main(Pixel2DIn input) : SV_Target0
{
	return float4(Material.Sample(LinearSampler,input.Uv).xyz,0.5);
}