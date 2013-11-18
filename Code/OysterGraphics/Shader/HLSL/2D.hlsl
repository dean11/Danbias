struct Vertex2DIn
{
	float2 Pos : Position;
};

cbuffer EveryObject2D : register(c0)
{
	float4x4 Translation;
};

SamplerState LinearSampler : register(s0);

struct Pixel2DIn
{
	float4 Pos : SV_Position;
	float2 Uv : TEXCOORD;
};

Texture2D Material : register(t0);

float4 ApplyMaterial0(Pixel2DIn input) : SV_Target0
{
	return Material.Sample(LinearSampler,input.Uv);
}


Vertex2DIn PassThrough(Vertex2DIn input)
{
	return input;
}

[maxvertexcount(4)]
void PointToQuad(point Vertex2DIn input[1],inout TriangleStream<Pixel2DIn> Quads)
{
	Pixel2DIn output;
	output.Pos = mul(float4(-1,-1,0,1) ,Translation);
	output.Uv = float2(0,1);
	Quads.Append(output);

	output.Pos = mul(float4(-1,1,0,1), Translation);
	output.Uv = float2(0,0);
	Quads.Append(output);

	output.Pos = mul(float4(1,-1,0,1), Translation);
	output.Uv = float2(1,1);
	Quads.Append(output);

	output.Pos = mul(float4(1,1,0,1), Translation);
	output.Uv = float2(1,0);
	Quads.Append(output);
}