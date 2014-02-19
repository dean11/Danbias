#include "BlurSharedData.hlsli"

[numthreads(1,N,1)]
void main(int3 ThreadID : SV_DispatchThreadID, int3 gThreadID : SV_GroupThreadID)
{

	if(gThreadID.y < blurRadius)
	{
		int y = max(ThreadID.y-blurRadius,0);
		gCache[gThreadID.y] = inTex[min(int2(ThreadID.x,y) + Start, Stop-1)];
	}
	if(gThreadID.y >= N-blurRadius)
	{
		int y = min(ThreadID.y +blurRadius, Stop.y-1);
		gCache[gThreadID.y+2*blurRadius] = inTex[int2(ThreadID.x,y) + Start];
	}
	gCache[gThreadID.y+blurRadius] = inTex[min(ThreadID.xy + Start, Stop-1)];

	GroupMemoryBarrierWithGroupSync();

	float4 blurCol = float4(0,0,0,0);

	[unroll]
	for(int i = -blurRadius; i <= blurRadius;++i)
	{
		int k = gThreadID.y + blurRadius +  i;
		blurCol +=Weights[i + blurRadius] * gCache[k];
	}

	outTex[min(ThreadID.xy + Start, Stop-1)] = blurCol * BlurMask + inTex[min(ThreadID.xy + Start, Stop-1)] * ( float4(1,1,1,1) - BlurMask);;
	//outTex[ThreadID.xy + Start] = inTex[ThreadID.xy+ Start];
}