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
	float3 tint = Color*(1-DiffGlow) + GlowColor * DiffGlow;
	tint = tint / 2;
	output.DiffuseGlow = DiffGlow * float4(tint,1);

	//NORMALIZE
	float3x3 CoTangentFrame = cotangent_frame(input.normal, normalize(input.ViewPos), input.UV);

	float3 map = Normal.Sample(S1,input.UV).xyz;
	//map = normalize((map * 2) -1);
	map = map * 255./127. - 128./127.;
	//map = map * 255;
	float3 normal = normalize(mul(CoTangentFrame, map));
	//normal = perturb_normal( normal, normalize(-input.ViewPos), input.UV );

	output.NormalSpec = float4(normal, Normal.Sample(S1, input.UV).w*255);

	//output.NormalSpec = float4(input.normal, Normal.Sample(S1, input.UV).w * 0);
	//output.NormalSpec = float4(map,0);
	return output;
}