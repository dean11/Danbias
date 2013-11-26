struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 ViewPos: POSITION;
	float2 UV : TEXCOORD;
	float4 normal : NORMAL;
};

struct PixelOut
{
	float4 Diffuse3Glow1		:	SV_TARGET0;
	float4 Normal3Specular1		:	SV_TARGET1;
	float4 VPos					: SV_TARGET2;
};

Texture2D Diffuse;
Texture2D Specular;

PixelOut main(VertexOut input)
{
	PixelOut output;
	output.Diffuse3Glow1 = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.Normal3Specular1 = float4(input.normal, 1.0f);
	return output;
}