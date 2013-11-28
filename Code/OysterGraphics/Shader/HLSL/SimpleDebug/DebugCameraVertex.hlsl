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
	float4 postTransform = mul( World, float4(input.pos,1) );
	//float4 postTransform = float4(input.pos,1);
	//return postTransform;
	//return mul(View, float4(input.pos,1));
	matrix VP = mul(Projection, View);
	//matrix WVP = mul(World, VP);
	return mul(VP, postTransform );
}