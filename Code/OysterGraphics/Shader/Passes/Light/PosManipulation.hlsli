#ifndef PosHelper
#define PosHelper
#include "Defines.hlsli"

//assumes ProperfloatTexCoords
float3 ToVpos(int2 texCoord, float2 UV)
{
	//clamp(UV, float2(-1,-1), float2(1,1));
	float4 ViewPos = float4(0,0,0,0);
	// Get the depth value for this pixel
	//texCoord = max(texCoord, uint2(0,0));
	//texCoord = min(texCoord, DepthTexture.Length);
	if(NoDepth[texCoord].x == 1.0f)
	{
		ViewPos.z= DepthTexture[texCoord].x;
	}
	else
	{
		ViewPos.z = NoDepth[texCoord].x;
	}
	//Get X/w
	ViewPos.x = UV.x;
	//Get Y/w
	//ViewPos.y = -(UV.y * 2) + 1;
	ViewPos.y = UV.y;
	ViewPos.w = 1;

	//Un project
	ViewPos = mul(InvProj, ViewPos);
	return ViewPos.xyz / ViewPos.w;
}
#endif