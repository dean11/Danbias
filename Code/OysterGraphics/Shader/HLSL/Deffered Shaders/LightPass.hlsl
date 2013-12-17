#include "Defines.hlsli"
#include "LightCalc.hlsli"
#include "PosManipulation.hlsli"
//todo
//LightCulling
//Calc Diff + Spec
//Calc Ambience
//Write Glow


[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float3 ViewPos = ToVpos(DTid.xy);
	DiffSpec Shaded;
	Shaded.Diffuse = float4(0,0,0,0);
	Shaded.Specular = float4(0,0,0,0);
	for(int i = 0; i < Lights; ++i)
	{
		DiffSpec light = LightCalc(Points[i], ViewPos, DTid.xy);
		Shaded.Diffuse = light.Diffuse;
		Shaded.Specular = light.Specular;
	}
	//Output[DTid.xy] = float4(ViewPos,1);
	//Output[DTid.xy] =  DepthTexture[DTid.xy].x;
	//Output[DTid.xy] = float4(DTid.xy/ Pixels,1,1);
	//Output[DTid.xy] = NormalSpec[DTid.xy];
	//Output[DTid.xy] = float4(light.Diffuse, 1);
	//Output[DTid.xy] = float4(light.Specular, 1);
	Output[DTid.xy] = float4( Shaded.Diffuse * DiffuseGlow[DTid.xy].xyz + /*Shaded.Specular +*/  DiffuseGlow[DTid.xy].xyz * 0.2f, 1);
}