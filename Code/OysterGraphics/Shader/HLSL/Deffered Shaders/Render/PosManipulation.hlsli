#include "Defines.hlsli"

//assumes ProperfloatTexCoords
float3 ToVpos(float2 texCoord)
{
	//Get proper UV
	float2 UV = float2(texCoord) / float2(Pixels);

	float4 ViewPos;
	// Get the depth value for this pixel
	ViewPos.z= DepthTexture[texCoord].x;
	//Get X/w
	ViewPos.x = UV.x * 2 - 1;
	//Get Y/w
	ViewPos.y = 1 - 2 * UV.y;
	ViewPos.w = 1;

	//Un project
	ViewPos = mul(ViewPos, InvProj);
	return ViewPos.xyz / ViewPos.w;
}