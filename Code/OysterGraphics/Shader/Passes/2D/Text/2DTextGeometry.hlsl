#include "Header.hlsli"

[maxvertexcount(4)]
void main(point Text2DIn input[1],inout TriangleStream<Pixel2DIn> Quads)
{
	float startoff=input[0].off*input[0].coff;
	float endoff=startoff+input[0].coff;
	Pixel2DIn output;

	output.Pos = mul(float4(-1,-1,0,1), Translation);
	output.Pos.x += input[0].Pos;
	output.Uv = float2(startoff,1);
	Quads.Append(output);

	output.Pos = mul(float4(-1,1,0,1), Translation);
	output.Pos.x += input[0].Pos;
	output.Uv = float2(startoff,0);
	Quads.Append(output);

	output.Pos = mul(float4(1,-1,0,1), Translation);
	output.Pos.x += input[0].Pos;
	output.Uv = float2(endoff,1);
	Quads.Append(output);

	output.Pos = mul(float4(1,1,0,1), Translation);
	output.Pos.x += input[0].Pos;
	output.Uv = float2(endoff,0);
	Quads.Append(output);
}