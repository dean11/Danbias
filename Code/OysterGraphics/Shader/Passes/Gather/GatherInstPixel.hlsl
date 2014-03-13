#include "InstHeader.hlsli"



float3x3 cotangent_frame( float3 N, float3 p, float2 uv )
{
	// get edge vectors of the pixel triangle
	float3 dp1 = ddx( p );
	float3 dp2 = ddy( p );
	float2 duv1 = ddx( uv );
	float2 duv2 = ddy( uv );
 
	// solve the linear system
	float3 dp2perp = cross( dp2, N );
	float3 dp1perp = cross( N, dp1 );
	float3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	float3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
	// construct a scale-invariant frame 
	float invmax = rsqrt( max( dot(T,T), dot(B,B) ) );
	return transpose(float3x3( T * invmax, B * invmax, N ));
}

float3 perturb_normal( float3 N, float3 V, float2 texcoord )
{
	// assume N, the interpolated vertex normal and 
	// V, the view vector (vertex to eye)
	float3 map = Normal.Sample(S1,texcoord).xyz;
	map = map * 255./127. - 128./127.;
#ifdef WITH_NORMALMAP_2CHANNEL
	map.z = sqrt( 1. - dot( map.xy, map.xy ) );
#endif
#ifdef WITH_NORMALMAP_GREEN_UP
	map.y = -map.y;
#endif
	float3x3 TBN = cotangent_frame( N, -V, texcoord );
	return normalize( mul(transpose(TBN), map) );
}

PixelOut main(VertexOut input)
{
	PixelOut output;
	float4 DiffGlow = Diffuse.Sample(S1, input.UV);
	float3 tint = input.tint * (1-DiffGlow.w) + input.gtint * DiffGlow.w;
	//tint =  Color*(1-DiffGlow.w) + GlowColor * DiffGlow.w;
	output.DiffuseGlow = DiffGlow * float4(tint,1);

	//NORMALIZE
	float3x3 CoTangentFrame = cotangent_frame(input.normal, normalize(input.ViewPos), input.UV);

	float3 map = Normal.Sample(S1,input.UV).xyz;
	//map = normalize((map * 2) -1);
	map = map * 255./127. - 128./127.;
	//map = map * 255;
	float3 normal = normalize(mul(CoTangentFrame, map));
	//normal = perturb_normal( normal, normalize(-input.ViewPos), input.UV );

	float Val = Normal.Sample(S1, input.UV).w*255;
	float mul = 0.5 + 0.5 * Val;
	//todo plot 1-45 in groups
	mul = Val > 7 ? 1.0/3 : mul;
	mul = Val > 20 ? 1.0/6 : mul;
	mul = Val > 45 ? (0.000000005417 * pow(Val,4)) - (0.000002849 * pow(Val,3)) + (0.00055562 * pow(Val,2)) - 0.0441362 * Val + 1.34392578: mul;
	output.NormalSpec = float4(normal, mul*Val);

	//output.NormalSpec = float4(input.normal, Normal.Sample(S1, input.UV).w * 0);
	//output.NormalSpec = float4(map,0);
	return output;
}