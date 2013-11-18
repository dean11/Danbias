cbuffer everyFrame : register(c0)
{
	matrix VP;
}

cbuffer everyObject : register(c1)
{
	matrix world;
}

Texture2D Diffuse  :register(t0);

float4 VSScene(float4 input : POSITION) : SV_Position
{
	//return input;
	return mul(input,VP);
}

float4 PSScene(float4 input : SV_Position) : SV_Target0
{
	return float4(0,1,0,1);
}