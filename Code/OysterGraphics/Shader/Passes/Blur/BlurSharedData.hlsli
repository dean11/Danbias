#ifndef BLURSHAREDDATA
#define BLURSHAREDDATA



static const float Weights[9] =
{
	0.05f, 0.05f, 0.1f, 0.15f, 0.3f, 0.15f, 0.1f, 0.05f, 0.05f
};

static const int blurRadius = 4;

#define N  128
#define gSize (N+2*blurRadius)
groupshared float4 gCache[gSize];

Texture2D inTex : register(t0);
RWTexture2D<float4> outTex : register(u0);

//cbuffer BlurrData : register(c0)
//{
//	static const int blurRadius = 5;
//	static const float  Weights[11] = 
//	{
//		0.05f,0.05f,0.1f,0.1f,0.1f,0.2f,0.1f,0.1f,0.1f,0.05f,0.05f
//	};
//};

//[numthreads(16,16,1)]
//void TryCompute(uint3 ThreadID : SV_DispatchThreadID)
//{
//	Output[ThreadID.xy] = Diffuse[ThreadID.xy]*0.5f+Glow[ThreadID.xy]*Glow[ThreadID.xy].w;
//}

#endif