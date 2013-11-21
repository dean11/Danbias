cbuffer PerFrame : register(b0)
{
	matrix View;
	float4x4 Projection;
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

float4 main( VertexIn input ) : SV_POSITION
{
	return float4(input.pos,1);
	return mul(float4(input.pos,1), View);
	matrix VP = mul(View, Projection);
	matrix WVP = mul(World, VP);
	return mul(WVP, float4(input.pos,1));
}