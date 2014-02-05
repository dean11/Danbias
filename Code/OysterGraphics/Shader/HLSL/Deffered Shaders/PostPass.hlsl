Texture2D Diffuse	: register(t0);
Texture2D Specular	: register(t1);
Texture2D Ambient	: register(t2);

RWTexture2D<float4> Output;

#define AmbFactor 0.3f;

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float4 Light  = Diffuse[DTid.xy] + Specular[DTid.xy];
	float4 Amb = float4(Ambient[DTid.xy/2].xyz * Ambient[DTid.xy/2].w,1);
	//Output[DTid.xy] = float4(Ambient[DTid.xy/2].xyz /* * Ambient[DTid.xy/2].w */, 1);
	//Output[DTid.xy] = Diffuse[DTid.xy] + Specular[DTid.xy];
	Output[DTid.xy] = Light + Amb * AmbFactor;
}