#include "BlurSharedData.hlsli"

[numthreads(N,1,1)]
void main(int3 ThreadID : SV_DispatchThreadID, int3 gThreadID : SV_GroupThreadID)
{

	if(gThreadID.x < blurRadius)
	{
		int x = max(ThreadID.x-blurRadius,0);
		gCache[gThreadID.x] = inTex[min(int2(x,ThreadID.y) + Start, Stop-1)];
	}
	if(gThreadID.x >= N-blurRadius)
	{
		int x = min(ThreadID.x+blurRadius,Stop.x-1);
		gCache[gThreadID.x+2*blurRadius] = inTex[int2(x,ThreadID.y) + Start];
	}
	gCache[gThreadID.x+blurRadius] = inTex[min(ThreadID.xy + Start, Stop-1)];

	GroupMemoryBarrierWithGroupSync();

	float4 blurCol = float4(0,0,0,0);

	[unroll]
	for(int i = -blurRadius; i <= blurRadius;++i)
	{
		int k = gThreadID.x + blurRadius +  i;
		blurCol +=Weights[i + blurRadius] * gCache[k];
	}

	outTex[min(ThreadID.xy + Start, Stop-1)] = blurCol * BlurMask + inTex[min(ThreadID.xy + Start, Stop-1)] * ( float4(1,1,1,1) - BlurMask);
	//outTex[ThreadID.xy + Start] = inTex[ThreadID.xy + Start];
}

