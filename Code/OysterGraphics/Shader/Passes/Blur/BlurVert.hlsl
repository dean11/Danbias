#include "BlurSharedData.hlsli"

[numthreads(1,N,1)]
void main(int3 ThreadID : SV_DispatchThreadID, int3 gThreadID : SV_GroupThreadID)
{

	if(gThreadID.y < blurRadius)
	{
		int y = max(ThreadID.y-blurRadius,0);
		gCache[gThreadID.y] = inTex[int2(ThreadID.x,y)];
	}
	if(gThreadID.y >= N-blurRadius)
	{
		int y = min(ThreadID.y+blurRadius,inTex.Length.y-1);
		gCache[gThreadID.y+2*blurRadius] = inTex[int2(ThreadID.x,y)];
	}
	gCache[gThreadID.y+blurRadius] = inTex[min(ThreadID.xy,inTex.Length.xy-1)];

	GroupMemoryBarrierWithGroupSync();

	float4 blurCol = float4(0,0,0,0);

	[unroll]
	for(int i = -blurRadius; i <= blurRadius;++i)
	{
		int k = gThreadID.y + blurRadius +  i;
		blurCol +=Weights[i + blurRadius] * gCache[k];
	}

	outTex[ThreadID.xy] = blurCol;
	//Output[ThreadID.xy] = inTex[((ThreadID.xy))];
}