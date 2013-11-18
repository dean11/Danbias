cbuffer Rarely : register(c0)
{
	matrix ViewProjection;
}

cbuffer everyFrame : register(c1)
{
	matrix View;
}

cbuffer everyObject : register(c2)
{
	matrix World;
}

Texture2D Diffuse  :register(t0);
Texture2D Specular :register(t1);
Texture2D Glow     :register(t2);

SamplerState LinearSampler : register(s0);

struct VSObjIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

struct PSObjIn
{
	float4 Pos : SV_POSITION;
	float3 WorldPos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

struct PSObjOut
{
	float4 Diffuse	: SV_Target0;
	float4 Specular	: SV_Target1;
	float4 Glow		: SV_Target2;
	float4 Pos		: SV_Target3;
	float4 Normal	: SV_Target4;
};

PSObjIn VSObj(VSObjIn input)
{
	PSObjIn output;
	matrix WV		= mul( World, View );
	matrix WVP		= mul (World, ViewProjection );
	output.Pos		= mul( float4(input.Pos, 1), WVP );
	output.WorldPos	= mul( float4(input.Pos, 1), WV ).xyz;
	output.Normal	= mul( float4(input.Normal, 0), WV ).xyz;
	output.UV		= input.UV;

	return output;
}

float4 PSObj (PSObjIn errors) : SV_Target0
{
	return Diffuse.Sample( LinearSampler, errors.UV );
}

PSObjOut PSDefObj(PSObjIn inp)
{
	PSObjOut outp;
	outp.Pos		= float4( inp.WorldPos, 1 );
	outp.Normal		= float4( normalize(inp.Normal), 0 ); // normalize since interpolation messes them up
	outp.Diffuse	= Diffuse.Sample( LinearSampler, inp.UV );
	outp.Specular	= Specular.Sample( LinearSampler, inp.UV );
	outp.Glow		= Glow.Sample( LinearSampler, inp.UV );

	return outp;
}