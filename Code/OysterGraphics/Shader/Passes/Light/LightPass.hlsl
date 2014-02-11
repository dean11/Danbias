#include "Defines.hlsli"
#include "LightCalc.hlsli"
#include "PosManipulation.hlsli"
#include "SSAO.hlsli"
//todo
//LightCulling
//Calc Diff + Spec	Done
//Calc Ambience		Done
//Write Glow


[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID )
{
	float2 UV = DTid.xy / Pixels;
	UV.x =	UV.x * 2 - 1;
	UV.y = 1 - 2 * UV.y;
	float3 ViewPos = ToVpos(DTid.xy, UV);
	DiffSpec Shaded;
	Shaded.Diffuse = float3(0,0,0);
	Shaded.Specular = float3(0,0,0);

	for(int i = 0; i < Lights; ++i)
	{
		DiffSpec light = LightCalc(Points[i], ViewPos, DTid.xy);
		Shaded.Diffuse += light.Diffuse;
		Shaded.Specular += light.Specular;
	}

	Diffuse[DTid.xy] = float4(Shaded.Diffuse * DiffuseGlow[DTid.xy].xyz,0);
	Specular[DTid.xy] = float4(Shaded.Specular, 0);

	
	if(DTid.x & 1 && DTid.y & 1 )
	{
		float AmbValue = GetSSAO(ViewPos, UV, DTid.xy, GTid.xy/2);
		Ambient[DTid.xy/2] = float4(DiffuseGlow[DTid.xy].xyz, AmbValue);
		Ambient[DTid.xy/2 + float2(Pixels.x/2, 0)] = GUI[DTid.xy];
		//Ambient[DTid.xy] = GUI[DTid.xy];
	}

}