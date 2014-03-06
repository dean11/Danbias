#include "Defines.hlsli"
#include "LightCalc.hlsli"
#include "PosManipulation.hlsli"
#include "SSAO.hlsli"
#include "TileCulling.hlsli"
#include "ReadSky.hlsli"





// ------------------------------------------------------------------ //

[numthreads(BLOCKSIZE, BLOCKSIZE, 1)]
void main( uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID,  uint3 Gid : SV_GroupID, uint GI : SV_GroupIndex )
{
	float2 UV = DTid.xy / (float2)Diffuse.Length.xy;
	UV.x =	UV.x * 2 - 1;
	UV.y = 1 - 2 * UV.y;

	float3 posN = float3(UV, DepthTexture[DTid.xy].x);
	
	float3 ViewPos = ToVpos(DTid.xy, UV);

	if(GI==0)
	{
		numVisiblePointLights = 0;
		iMinDepth = 0x7F7FFFFF;
		iMaxDepth = 0;
	}
	GroupMemoryBarrierWithGroupSync();

	bool ValidPixel = true;
	if(posN.z == 1.0f && NoDepth[DTid.xy].x == 1.0f)
	{
		ValidPixel = false;
	}

	CullLights(Gid,GI,ValidPixel, ViewPos.z);


	
	DiffSpec Shaded;
	Shaded.Diffuse = float3(0,0,0);
	Shaded.Specular = float3(0,0,0);
	Shaded.Ambient = float3(1,1,1);

	for(int i = 0; i < numVisiblePointLights; ++i)
	//for(int i = 0; i < Lights; ++i)
	{
		DiffSpec light = LightCalc(Points[visiblePointlightIndex[i]], ViewPos, DTid.xy);
		//DiffSpec light = LightCalc(Points[i], ViewPos, DTid.xy);
		Shaded.Diffuse += light.Diffuse;
		Shaded.Specular += light.Specular;
		Shaded.Ambient += light.Ambient;
	}

	Diffuse[DTid.xy] = float4(Shaded.Diffuse * DiffuseGlow[DTid.xy].xyz,1);
	Specular[DTid.xy] = float4(Shaded.Specular, 0);

	
	if(DTid.x & 1 && DTid.y & 1 )
	{
		float AmbValue;
		float4 DiffBase = 0;

		if(ValidPixel)
		{
			AmbValue = GetSSAO(ViewPos, UV, DTid.xy, GTid.xy/2);

			DiffBase = DiffuseGlow[DTid.xy];
			DiffBase += DiffuseGlow[DTid.xy + uint2(1,0)];
			DiffBase += DiffuseGlow[DTid.xy + uint2(0,1)];
			DiffBase += DiffuseGlow[DTid.xy + uint2(1,1)];
			DiffBase = DiffBase / 4;
			DiffBase *= float4(Shaded.Ambient, 1);
		}
		else
		{
			AmbValue = 1;
			float3 ViewVec = mul(transpose(View), float4(ViewPos,0));
			DiffBase = float4(ReadSky(ViewVec).xyz*(1/AmbFactor), 0);
		}

		Ambient[DTid.xy/2] = float4(DiffBase.xyz , AmbValue);
		//Ambient[DTid.xy/2] = float4(DiffBase.xyz, 1);
		Ambient[DTid.xy/2 + float2(Diffuse.Length.x/2, 0)] = GUI[DTid.xy];
		Ambient[DTid.xy/2 + float2(0, Diffuse.Length.y/2)] = float4(DiffBase.xyz * DiffBase.w ,DiffBase.w);
		Ambient[DTid.xy/2 + Diffuse.Length.xy/2] = float4(numVisiblePointLights * (1.0f/Lights), 0, 0 ,1);
		Ambient[DTid.xy/2 + Diffuse.Length.xy/2] = float4(NormalSpec[DTid.xy].xyz ,1);
		//Ambient[DTid.xy/2 + Diffuse.Length.xy/2] = float4(Points[0].Pos ,1);
	}

}