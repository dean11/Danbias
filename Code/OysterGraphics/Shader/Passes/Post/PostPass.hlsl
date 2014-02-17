Texture2D Diffuse	: register(t0);
Texture2D Specular	: register(t1);
Texture2D Ambient	: register(t2);

RWTexture2D<float4> Output;

SamplerState S1 : register(s0);

cbuffer Size : register(b0)
{
	int2 Pixels;
}

#define AmbFactor 0.1f;

float4 SuperSample(float4 Glow, uint3 DTid)
{
	// Line X
	float2 index  = (float2)(DTid.xy/2);
	index += float2(0,Output.Length.y/2);
	index = index / Output.Length;
	Glow = Ambient.SampleLevel(S1, index,1);
	Glow = Glow * Glow.w*10;

	return Glow;
}

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float4 Light = Diffuse[DTid.xy] + saturate(Specular[DTid.xy]);
	float4 Amb = float4(Ambient[DTid.xy/2].xyz /* * Ambient[DTid.xy/2].w*/, 0);
	//float4 Glow =  Ambient[DTid.xy/2 + uint2(0,Output.Length.y/2)];
	float4 Glow =  Ambient[DTid.xy/2 + uint2(0,Output.Length.y/2)];
	
	Glow = SuperSample(Glow,DTid);

	float4 GUI;
	uint2 index = DTid.xy/2 + uint2((uint)Pixels.x/(uint)2,0);
	float3 PostLight = Amb.xyz * AmbFactor;
	PostLight = PostLight + Light.xyz + Glow;
	GUI = float4(Ambient[index]);
	PostLight = PostLight * (1 - GUI.w);
	Output[DTid.xy] = float4((GUI.xyz * GUI.w) + PostLight, 1);

	//Output[DTid.xy] = float4(Ambient[DTid.xy/2 + uint2(0,Output.Length.y*0.5f)].xyz,1);
}