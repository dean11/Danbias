#include "Debug.hlsl"

VertexOut main( VertexIn input )
{
	VertexOut outp;
	outp.Wpos = mul( World, float4(input.pos,1) );
	matrix VP = mul(Projection, View);
	outp.Pos = mul(VP, outp.Wpos );
	outp.UV = input.UV;
	outp.Normal = input.normal;

	return outp;
}