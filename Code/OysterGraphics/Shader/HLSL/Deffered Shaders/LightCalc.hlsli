#include "Defines.hlsli"

DiffSpec LightCalc(PointLight pl, float3 pos, int2 texCoord)
{
	DiffSpec output;
	float4 normalSpec = NormalSpec[texCoord];
	float3 lightVec = pl.Pos - pos.xyz;
	float d = length(lightVec);
	lightVec = lightVec/d;

	float diffFactor = max(dot(lightVec, normalSpec.xyz), 0.0f);
	float3 v  = reflect(-lightVec, normalSpec.xyz);
	float specFactor = pow(max(dot(v,normalize(-pos)), 0.0f),normalSpec.w);
	//Check att later
	float att = max( 0, 1 - (d / pl.Radius));
	//att = 1;
	//fix Ilum calcs instead of PhongBlinn
	output.Diffuse = pl.Bright * att * diffFactor * pl.Color;
	output.Specular = pl.Bright * att * specFactor * pl.Color;
	if(diffFactor == 0)
		output.Specular * 0;
	if(d > pl.Radius)
	{
		output.Diffuse = float4(0,0,0,1);
		output.Specular = float4(0,0,0,1);
	}
	return output;  
}