#include "Defines.hlsli"

//assumes ProperfloatTexCoords
float3 ToVpos(float2 texCoord)
{
	//Get proper UV
	float2 UV = texCoord / Pixels;

	float4 ViewPos = float4(0,0,0,0);
	// Get the depth value for this pixel
	ViewPos.z= DepthTexture[texCoord].x;
	//Get X/w
	ViewPos.x = UV.x * 2 - 1;
	//Get Y/w
	//ViewPos.y = -(UV.y * 2) + 1;
	ViewPos.y = 1 - 2 * UV.y;
	ViewPos.w = 1;

	//Un project
	ViewPos = mul(InvProj, ViewPos);
	return ViewPos.xyz / ViewPos.w;
}