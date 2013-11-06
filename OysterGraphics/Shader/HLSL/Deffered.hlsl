Texture2D Diffuse : register(t0);
Texture2D Glow : register(t1);

RWTexture2D<float4> Output : register(u0);

cbuffer BlurrData : register(c0)
{
	static const int blurRadius = 5;
	static const float  Weights[11] = 
	{
		0.05f,0.05f,0.1f,0.1f,0.1f,0.2f,0.1f,0.1f,0.1f,0.05f,0.05f
	};
};

[numthreads(32,32,1)]
void TryCompute(uint3 ThreadID : SV_DispatchThreadID)
{
	Output[ThreadID.xy] = Diffuse[ThreadID.xy]*0.5f+Glow[ThreadID.xy]*Glow[ThreadID.xy].w;
}

#define N  128
#define gSize (N+2*blurRadius)
groupshared float4 gCache[gSize];

[numthreads(N,1,1)]
void BlurrHor(int3 ThreadID : SV_DispatchThreadID, int3 gThreadID : SV_GroupThreadID)
{

	if(gThreadID.x < blurRadius)
	{
		int x = max(ThreadID.x-blurRadius,0);
		gCache[gThreadID.x] = Diffuse[int2(x,ThreadID.y)];
	}
	if(gThreadID.x >= N-blurRadius)
	{
		int x = min(ThreadID.x+blurRadius,Diffuse.Length.x-1);
		gCache[gThreadID.x+2*blurRadius] = Diffuse[int2(x,ThreadID.y)];
	}
	gCache[gThreadID.x+blurRadius] = Diffuse[min(ThreadID.xy,Diffuse.Length.xy-1)];

	GroupMemoryBarrierWithGroupSync();

	float4 blurCol = float4(0,0,0,0);

	[unroll]
	for(int i = -blurRadius; i <= blurRadius;++i)
	{
		int k = gThreadID.x + blurRadius +  i;
		blurCol +=Weights[i + blurRadius] * gCache[k];
	}

	Output[ThreadID.xy] = blurCol;
	//Output[ThreadID.xy] = Diffuse[((ThreadID.xy))];
}

[numthreads(1,N,1)]
void BlurrVert(int3 ThreadID : SV_DispatchThreadID, int3 gThreadID : SV_GroupThreadID)
{

	if(gThreadID.y < blurRadius)
	{
		int y = max(ThreadID.y-blurRadius,0);
		gCache[gThreadID.y] = Diffuse[int2(ThreadID.x,y)];
	}
	if(gThreadID.y >= N-blurRadius)
	{
		int y = min(ThreadID.y+blurRadius,Diffuse.Length.y-1);
		gCache[gThreadID.y+2*blurRadius] = Diffuse[int2(ThreadID.x,y)];
	}
	gCache[gThreadID.y+blurRadius] = Diffuse[min(ThreadID.xy,Diffuse.Length.xy-1)];

	GroupMemoryBarrierWithGroupSync();

	float4 blurCol = float4(0,0,0,0);

	[unroll]
	for(int i = -blurRadius; i <= blurRadius;++i)
	{
		int k = gThreadID.y + blurRadius +  i;
		blurCol +=Weights[i + blurRadius] * gCache[k];
	}

	Output[ThreadID.xy] = blurCol;
	//Output[ThreadID.xy] = Diffuse[((ThreadID.xy))];
}