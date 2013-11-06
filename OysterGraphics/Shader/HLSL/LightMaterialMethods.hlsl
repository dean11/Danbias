// Personal notes and trail of thought //////////////////////////////
/*	
	Light is flux ( latin fluxus ; flow )
	(F1) Flux density : amount of flow through a cross section (A)
	(L) Luminosity: property of light, amount of light output per time. Equivalent with the Watt unit.
	(A) Area as a function of range(r).

	A[r] = 4*PI*r^2
	F1 = L / A[r] = L / ( 4*PI*r^2 )

	_Attenuation_
	Attenuation is the gradual loss in intensity of any kind of flux through a medium. ~ Wikipedia
	It is also dependant of BOTH medium material and type of Flux. Attenuation( soft Tissue, Light ) != Attenuation( soft Tissue, Xray ).
	Hm'Yeah .. lets not use that. And SHAME to those who use that word carelessly in computer graphics!!!

	_Attenuation_coefficient_
	The linear intensity loss of a narrow beam through a medium.
	Hm .. This sounds CG delicious :)
	F2 = L / ( AttCoeff * r )

	_Total:_
	F = F1 + F2 = L / ( 4*PI*r^2 ) + L / ( AttCoeff * r ) = L / ( AttCoeff*r + 4*PI*r^2 )
									May vary between light spreadTypes 	-------^^^^

	L = ( Red Luminosity, Green Luminosity, Blue Luminosity, Spread )
	F = ( Red Density, Green Density, Blue Density ) = L.rgb / (AttCoeff*r + L.s^2)

	AttCoeff : property of Enviroment
	Ambience Light : property of Enviroment
*/
#ifndef LIGHTMATERIALMETHODS_HLSL
#define LIGHTMATERIALMETHODS_HLSL

#include "CollisionMethods.hlsl"

#define PI						3.1415926535897932384626433832795f
#define POINTLIGHT_SPREADCOEFF	12.566370614359172953850573533118f

struct PointLight
{
	Sphere pos;
	float3 color;
	float intensity;
};

void accumulateLight( inout float3 diffusePixel, inout float3 specularPixel, uniform in PointLight lightV, uniform in float exposure, uniform in float specularGloss, uniform in float3 pixelPosV, uniform in float3 normalV )
{
	float3 toLightVecV = lightV.pos.center - pixelPosV;
	float range = length( toLightVecV );

	if( !(range <= lightV.pos.radius ) )
	{
		return;
	}

	toLightVecV /= range;

	float3 illum = lightV.color;
	illum *= (exposure * lightV.intensity) / ( range + (POINTLIGHT_SPREADCOEFF * pow(range, 2.0f)) ); // light density on all rgb channels

	// calculating and adding the DiffuseCoefficient
	float coeff = max( dot(toLightVecV, normalV), 0.0f );
	diffusePixel += illum * coeff;

	if( coeff > 0.0f )
	{ // calculating and adding the SpecularCoefficient
		coeff = max( dot( reflect(-toLightVecV, normalV), normalize(-pixelPosV) ), 0.0f );
		specularPixel += illum * pow( coeff, specularGloss );
	}
	
}

// ------------------ old below
/*
struct Medium
{
	float3 ambienceLuminosity;
	float attenuationCoeff;
};

struct Light
{
	matrix worldToLightVolume;
	float3 luminosity;
	float spreadCoeff;
	float4 orientation;
};

// returns light density per rgb channel
float3 calcLightDensity( uniform float rangeW, uniform Light light, uniform Medium enviroment )
{ return light.luminosity / ( (enviroment.attenuationCoeff * rangeW) + (light.spreadCoeff * pow(rangeW, 2.0f)) ); }

// returns light density per rgb channel
float3 calcLightDensity( uniform float rangeW, uniform Light light )
{ return light.luminosity / ( rangeW + (light.spreadCoeff * pow(rangeW, 2.0f)) ); }

float calcDiffuseCoeff( uniform float3 normalW, uniform float3 toLightVecW )
{ return max( dot( toLightVecW, normalW ), 0.0f ); }

float calcSpecularCoeff( uniform float3 normalW, uniform float3 toLightVecW, uniform float3 toObserverVecW )
{ return max( dot( reflect( -toLightVecW, normalW ), toObserverVecW ), 0.0f ); }

/////////////////////////////////////////////////////////////////////
// SHADOW SAMPLING
/////////////////////////////////////////////////////////////////////
/*SamplerComparisonState shadowSampling
{
	Filter = COMPARISON_MIN_MAG_MIP_POINT;
	ComparisonFunc = LESS_EQUAL;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4( 0.0f, 0.0f, 0.0f, 0.0f );
};/**/
/*
SamplerState shadowSampling
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4( 0.0f, 0.0f, 0.0f, 0.0f );
};/**/
/*
void sampleShadowPCFx4( uniform float3 posW, uniform Light light, uniform Texture2D shadowMap, uniform float2 shadowMapResolution, out float lightExposure, out float range )
{
	float4 value = mul( light.worldToLightVolume, float4(posW, 1.0f) );
	value /= value.w;
	float2 shadowUV = 0.5f*( float2(value.x, -value.y) + 1.0f );
	float2 shadowUVDelta = 1.0f / shadowMapResolution;
	range = value.z - 0.00390625f;
	
	//lightExposure = shadowMap.SampleCmpLevelZero( shadowSampling, shadowUV, range ).r;
	value.x = (float)shadowMap.Sample(shadowSampling, shadowUV ).r;
	value.y = (float)shadowMap.Sample(shadowSampling, shadowUV + float2(shadowUVDelta.x, 0.0f) );
	value.z = (float)shadowMap.Sample(shadowSampling, shadowUV + float2(0.0f, shadowUVDelta.y) );
	value.w = (float)shadowMap.Sample(shadowSampling, shadowUV + shadowUVDelta );
	
	value.x = range > value.x ? 0.0f : 1.0f; // 1.0f if lightRange is not lesser than range. Else 0.0f
	value.y = range > value.y ? 0.0f : 1.0f;
	value.z = range > value.z ? 0.0f : 1.0f;
	value.w = range > value.w ? 0.0f : 1.0f;

	range += 0.00390625f;
	
	shadowUV = frac( shadowUV * shadowMapResolution );
	lightExposure = lerp( lerp( value.x, value.y, shadowUV.x ), lerp( value.z, value.w, shadowUV.x ), shadowUV.y );
}
*/

#endif