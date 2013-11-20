cbuffer PerFrame : register(b0)
{
	matrix View;
	float4x4 Projection;
}

cbuffer PerModel : register(b1)
{
	matrix World;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	matrix VP = mul(View, Projection);
	matrix WVP = mul(World, VP);
	return mul(WVP, pos);
}