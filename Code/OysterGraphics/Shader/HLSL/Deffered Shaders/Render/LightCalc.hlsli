#include "Defines.hlsli"

DiffSpec LightCalc(PointLight pl, float3 pos, int2 texCoord)
{
	DiffSpec output;
	float4 normalSpec = NormalSpec[texCoord];
	float3 lightVec = pl.PosRadius.xyz - pos.xyz;
	float d = length(lightVec);
	lightVec = lightVec/d;

	float diffFactor = max(dot(lightVec, normalSpec.xyz), 0.0f);
	float3 v  = reflect(-lightVec, normalSpec.xyz);
	float specFactor = pow(max(dot(v,normalize(-pos)), 0.0f),normalSpec.w);
	//Check att later
	float att = (max(d-pl.PosRadius.w,0)/pow(pl.PosRadius.w,2));

	//fix Ilum calcs instead of PhongBlinn
	output.Diffuse = pl.ColorBright.w * att * diffFactor * pl.ColorBright.xyz;
	output.Specular = pl.ColorBright.w * att * specFactor * pl.ColorBright.xyz;
	if(diffFactor == 0)
		output.Specular * 0;
	return output;  
}