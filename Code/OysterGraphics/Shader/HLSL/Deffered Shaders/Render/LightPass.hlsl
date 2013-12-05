#include "Defines.hlsli"
#include "LightCalc.hlsli"
#include "PosManipulation.hlsli"
//todo
//LightCulling
//Calc Diff + Spec
//Calc Ambience
//Write Glow


[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float3 ViewPos = ToVpos(DTid.xy);
	//DiffSpec LightCalc(pl, float3 pos)
}