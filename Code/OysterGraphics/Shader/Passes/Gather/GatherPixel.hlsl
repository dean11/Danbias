#include "Header.hlsli"



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
	float invmax = 1/sqrt( max( dot(T,T), dot(B,B) ) );
	return float3x3( T * invmax, B * invmax, N );
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
	output.DiffuseGlow = Diffuse.Sample(S1, input.UV);
	float3 normal = normalize(input.normal);

	normal = perturb_normal( normal, normalize(-input.ViewPos), input.UV );

	output.NormalSpec = float4(normal, Normal.Sample(S1,input.UV).w);
	return output;
}