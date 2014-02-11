#include "Header.hlsli"

[maxvertexcount(4)]
void main(point Vertex2DIn input[1],inout TriangleStream<Pixel2DIn> Quads)
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