#ifndef BLURSHAREDDATA
#define BLURSHAREDDATA



cbuffer BlurrData : register(b1)
{
	//static const int blurRadius = 0;
	//static const float Weights[1] =
	//{
	//	1.0f
	//};
	/*static const int blurRadius = 4;
	static const float Weights[9] =
	{
		0.05f, 0.05f, 0.1f, 0.15f, 0.3f, 0.15f, 0.1f, 0.05f, 0.05f
	};*/
	static const int blurRadius = 5;
	static const float  Weights[11] = 
	{
		0.05f,0.05f,0.1f,0.1f,0.1f,0.2f,0.1f,0.1f,0.1f,0.05f,0.05f
	};
};



#define N  128
#define gSize (N+2*blurRadius)
groupshared float4 gCache[gSize];

Texture2D inTex : register(t0);
RWTexture2D<float4> outTex : register(u0);

cbuffer BlurrData : register(b0)
{
	uint2 Start;
	uint2 Stop;
	float4 BlurMask;
};


//[numthreads(16,16,1)]
//void TryCompute(uint3 ThreadID : SV_DispatchThreadID)
//{
//	Output[ThreadID.xy] = Diffuse[ThreadID.xy]*0.5f+Glow[ThreadID.xy]*Glow[ThreadID.xy].w;
//}

#endif