Texture2D Diffuse	: register(t0);
Texture2D Specular	: register(t1);
Texture2D Ambient	: register(t2);

RWTexture2D<float4> Output;

SamplerState S1 : register(s0);

cbuffer Size : register(b0)
{
	float AmbFactor;
	float3 Color;
	float3 GlowColor;
}

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float SSAO = Ambient[DTid.xy/2].w;
	for(int x = 0; x < 4; ++x)
	{
		for(int y = 0; y < 4; ++y)
		{
			SSAO += Ambient[DTid.xy/2 + uint2(-2+x,-2+y)].w;
		}
	}

	SSAO = SSAO / 16;

	float4 Light = Diffuse[DTid.xy] + saturate(Specular[DTid.xy]);
	float3 Amb =  Ambient[DTid.xy/2].xyz  * SSAO * Color;
	
	float4 Glow =  Ambient[DTid.xy/2 + uint2(0,Output.Length.y/2)];
	Glow = float4(Glow.xyz * GlowColor, 1);

	float4 GUI;
	uint2 index = DTid.xy/2 + uint2((uint)Output.Length.x/(uint)2,0);
	float3 PostLight = Amb * AmbFactor;
	PostLight = PostLight + Light.xyz + Glow;
	GUI = float4(Ambient[index]);
	PostLight = PostLight * (1 - GUI.w);
	Output[DTid.xy] = float4((GUI.xyz * GUI.w) + PostLight, 1);

	//Output[DTid.xy] = float4(Ambient[DTid.xy/2 + uint2(Output.Length*0.5f)].xyz,1);
	//Output[DTid.xy] = SSAO * float4(1,1,1,1);
	//Output[DTid.xy] = Ambient[DTid.xy];
	//Output[DTid.xy] = Diffuse[DTid.xy];
}