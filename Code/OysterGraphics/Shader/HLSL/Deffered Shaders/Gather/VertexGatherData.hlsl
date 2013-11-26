cbuffer PerFrame : register(b0)
{
	matrix View;
	float4x4 Projection;
	matrix VP;
}

cbuffer PerModel : register(b1)
{
	matrix World;
}

struct VertexIn
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 ViewPos: POSITION;
	float2 UV : TEXCOORD;
	float4 normal : NORMAL;
};

VertexOut main( VertexIn input )
{
	VertexOut output;
	matrix WV = mul(View, World);
	output.ViewPos = mul(WV, float4(input.pos,1));
	output.pos = mul(Projection, output.ViewPos);
	output.UV = input.UV;
	output.normal = float4(input.normal, 0);
	return output;
}