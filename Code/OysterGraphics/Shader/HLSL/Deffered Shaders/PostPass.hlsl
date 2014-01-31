Texture2D Diffuse	: register(t0);
Texture2D Specular	: register(t1);
Texture2D Ambient	: register(t2);

RWTexture2D<float4> Output;

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	Output[DTid.xy] = Diffuse[DTid.xy] + Specular[DTid.xy];// + Diffuse[DTid.xy] * Ambient[DTid.xy/4].w;// + float4(Ambient[DTid.xy/4].xyz,1); GLOW
	//Output[DTid.xy] = Diffuse[DTid.xy] + Diffuse[DTid.xy] * Ambient[DTid.xy/4].w;// + float4(Ambient[DTid.xy/4].xyz,1); GLOW
	
	//Output[DTid.xy] = Diffuse[DTid.xy];
}