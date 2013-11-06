#include "2D.hlsl"

cbuffer TextPerObject : register(c0)
{
	float4x4 gWorld;
};

Texture2D g_tex1 : register(t0);

struct TEXT_VS_IN
{
	float3 pos    : POSITION;
	float2 texCoord : TEXCOORD;
	float ch : CHAR;
	float3 off: VOFFSET;
};

struct TEXT_VS_OUT
{
	float4 pos    : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

TEXT_VS_OUT TEXT_VS(TEXT_VS_IN vIn)
{
	TEXT_VS_OUT vOut;
	vOut.pos = mul(float4(vIn.pos+vIn.off, 1.0f), gWorld);
	
	//Adding texCoord
	vOut.texCoord  = vIn.texCoord;
	vOut.texCoord[0]+=vIn.ch;
	return vOut;
}


float4 TEXT_PS(TEXT_VS_OUT pIn) : SV_Target0
{
	return g_tex1.Sample(LinearSampler, pIn.texCoord);
}

struct Text2DIn
{
	float Pos : Position;
	int off : Offset;
	float coff : CharOffset;
};
Text2DIn PassThroughText(Text2DIn input)
{
	return input;
}
[maxvertexcount(4)]
void TextToQuad(point Text2DIn input[1],inout TriangleStream<Pixel2DIn> Quads)
{
	float startoff=input[0].off*input[0].coff;
	float endoff=startoff+input[0].coff;
	Pixel2DIn output;

	output.Pos = mul(float4(-1,-1,0,1), Translation);
	output.Pos.x+=input[0].Pos;
	output.Uv = float2(startoff,1);
	Quads.Append(output);

	output.Pos = mul(float4(-1,1,0,1), Translation);
	output.Pos.x+=input[0].Pos;
	output.Uv = float2(startoff,0);
	Quads.Append(output);

	output.Pos = mul(float4(1,-1,0,1), Translation);
	output.Pos.x+=input[0].Pos;
	output.Uv = float2(endoff,1);
	Quads.Append(output);

	output.Pos = mul(float4(1,1,0,1), Translation);
	output.Pos.x+=input[0].Pos;
	output.Uv = float2(endoff,0);
	Quads.Append(output);
}