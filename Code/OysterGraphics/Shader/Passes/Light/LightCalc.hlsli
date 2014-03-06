#include "Defines.hlsli"

DiffSpec LightCalc(PointLight pl, float3 pos, int2 texCoord)
{
	DiffSpec output;
	float4 normalSpec = NormalSpec[texCoord];
	float4 LPos = float4(pl.Pos, 1);
	float3 lightVec = LPos.xyz - pos;
	float d = length(lightVec);
	
	lightVec = lightVec/d;
	
	if(d > pl.Radius)
	{
		output.Diffuse = float3(0,0,0);
		output.Specular = float3(0,0,0);
		output.Ambient = float3(0,0,0);
		Ambient[texCoord.xy/2 + Diffuse.Length.xy/2] = float4(1,0,0 ,1);
		return output;
	}

	float diffFactor = max(dot(lightVec, normalSpec.xyz), 0.0f);
	float3 v  = reflect(-lightVec, normalSpec.xyz);
	float specFactor = pow(max(dot(v,normalize(-pos)), 0.0f),normalSpec.w);
	//Check att later
	float att = max( 0, 1 - ((d ) / pl.Radius));
	//fix Ilum calcs instead of PhongBlinn
	output.Diffuse = pl.Bright * att * diffFactor *  pl.Color;
	output.Specular = pl.Bright * att * specFactor * pl.Color;
	if(diffFactor == 0)
		output.Specular * 0;
	Ambient[texCoord.xy/2 + Diffuse.Length.xy/2] = float4(0,1,0 ,1);
	float SpecCo = normalSpec.w < 1 ? 0.0f : 1.0f;
	output.Ambient = pl.Bright * att * AmbFactor * pl.Color;
	output.Specular = output.Specular * SpecCo;
	return output;  
}