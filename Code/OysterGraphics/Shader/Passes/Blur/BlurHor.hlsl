#include "BlurSharedData.hlsli"

[numthreads(N,1,1)]
void main(int3 ThreadID : SV_DispatchThreadID, int3 gThreadID : SV_GroupThreadID)
{

	if(gThreadID.x < blurRadius)
	{
		int x = max(ThreadID.x-blurRadius,0);
		gCache[gThreadID.x] = inTex[int2(x,ThreadID.y)];
	}
	if(gThreadID.x >= N-blurRadius)
	{
		int x = min(ThreadID.x+blurRadius,inTex.Length.x-1);
		gCache[gThreadID.x+2*blurRadius] = inTex[int2(x,ThreadID.y)];
	}
	gCache[gThreadID.x+blurRadius] = inTex[min(ThreadID.xy,inTex.Length.xy-1)];

	GroupMemoryBarrierWithGroupSync();

	float4 blurCol = float4(0,0,0,0);

	[unroll]
	for(int i = -blurRadius; i <= blurRadius;++i)
	{
		int k = gThreadID.x + blurRadius +  i;
		blurCol +=Weights[i + blurRadius] * gCache[k];
	}

	outTex[ThreadID.xy] = blurCol;
	//Output[ThreadID.xy] = Diffuse[((ThreadID.xy))];
}

