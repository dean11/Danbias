struct VertexIn
{
	float3 Pos : Position;
};

cbuffer EveryObject2D : register(c0)
{
	float4x4 VP;
};

struct Pixel2DIn
{
	float4 Pos : SV_Position;
	float2 Uv : TEXCOORD;
};

const static float Width = 5.0f;

VertexIn PassThrough(VertexIn input)
{
	return input;
}

[maxvertexcount(16)]
void Lazer(line VertexIn input[2],inout TriangleStream<Pixel2DIn> Quads)
{
	//build coordinate system
	float3 r =normalize(input[1].Pos-input[0].Pos);
	float3 s = abs(r);
	//set s ortogonal to r
	if(s.x<s.y && s.x<s.z)
		s = float3(0,-r.z,r.y);
	else if(s.y<s.x && s.y<s.z)
		s = float3(-r.z,0,r.x);
	else 
		s = float3(-r.y,r.x,0);

	float3 t = cross(r,s);

	
	Pixel2DIn output;
	//create the corners of the plane rxs Forward Facing
	//top left
	output.Pos = mul(float4( input[0].Pos + Width*s,1),VP);
	output.Uv = float2(0,1);
	Quads.Append(output);
	//botom left
	output.Pos = mul(float4( input[0].Pos - Width*s,1),VP);
	output.Uv = float2(0,0);
	Quads.Append(output);

	//top right
	output.Pos = mul(float4( input[1].Pos + Width*s,1),VP);
	output.Uv = float2(1,1);
	Quads.Append(output);

	//botom right
	output.Pos =mul(float4( input[1].Pos - Width*s,1),VP);
	output.Uv = float2(1,0);
	Quads.Append(output);

	Quads.RestartStrip();

	//create the corners of the plane rxs BackWard Facing
	//top left
	output.Pos = mul(float4( input[0].Pos + Width*s,1),VP);
	output.Uv = float2(0,1);
	Quads.Append(output);

	//top right
	output.Pos = mul(float4( input[1].Pos + Width*s,1),VP);
	output.Uv = float2(1,1);
	Quads.Append(output);

	//botom left
	output.Pos = mul(float4( input[0].Pos - Width*s,1),VP);
	output.Uv = float2(0,0);
	Quads.Append(output);

	//botom right
	output.Pos =mul(float4( input[1].Pos - Width*s,1),VP);
	output.Uv = float2(1,0);
	Quads.Append(output);

	Quads.RestartStrip();

	//create the corners of the plane rxt Forward Facing
	//top left
	output.Pos = mul(float4( input[0].Pos + Width*t,1),VP);
	output.Uv = float2(0,1);
	Quads.Append(output);
	//botom left
	output.Pos = mul(float4( input[0].Pos - Width*t,1),VP);
	output.Uv = float2(0,0);
	Quads.Append(output);

	//top right
	output.Pos = mul(float4( input[1].Pos + Width*t,1),VP);
	output.Uv = float2(1,1);
	Quads.Append(output);

	//botom right
	output.Pos =mul(float4( input[1].Pos - Width*t,1),VP);
	output.Uv = float2(1,0);
	Quads.Append(output);

	Quads.RestartStrip();

	//create the corners of the plane rxt Forward Facing
	//top left
	output.Pos = mul(float4( input[0].Pos + Width*t,1),VP);
	output.Uv = float2(0,1);
	Quads.Append(output);

	//top right
	output.Pos = mul(float4( input[1].Pos + Width*t,1),VP);
	output.Uv = float2(1,1);
	Quads.Append(output);

	//botom left
	output.Pos = mul(float4( input[0].Pos - Width*t,1),VP);
	output.Uv = float2(0,0);
	Quads.Append(output);

	//botom right
	output.Pos =mul(float4( input[1].Pos - Width*t,1),VP);
	output.Uv = float2(1,0);
	Quads.Append(output);
}