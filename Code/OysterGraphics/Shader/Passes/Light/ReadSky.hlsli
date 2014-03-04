#include "Defines.hlsli"

float4 ReadSky(float3 d)
{
	//Massive if to find largest component
	if( abs(d.z) > abs(d.x) && abs(d.z) > abs(d.y))
	{
		//from -1 - 1 to 0 - 1
		float2 UV = float2(d.x/d.z,d.y/d.z);
		UV = UV * 0.5 + 0.5;
		//looking back or front
		if(d.z > 0)
		{
			UV.y = 1 - UV.y;
			return Front[uint2(UV * Up.Length)];
		}


		return Back[uint2(UV * Up.Length)];
	}
	else
	{
		if(abs(d.y) > abs(d.x) && abs(d.y) > abs(d.z))
		{
			float2 UV =float2(d.z/d.y,d.x/d.y);
			UV = UV * 0.5 + 0.5;
			//UV.y = -UV.y;
			//looking up or down
			if(d.y>0)
			{
				UV.y = 1 - UV.y;
				return Up[uint2(UV * Up.Length)];
			}

			return Down[uint2(UV * Up.Length)];
		}
		else
		{
			if(abs(d.x) > abs(d.y) && abs(d.x) > abs(d.z))
			{
				
				float2 UV = float2(d.z/d.x,d.y/d.x);
				UV = UV * 0.5 + 0.5;
				UV.y = 1 - UV.y;
				UV.x = 1 - UV.x;
				//looking left and right
				if(d.x > 0)
				{
					return Left[uint2(UV * Up.Length)];
				}
				UV.y = 1 - UV.y;
				return Right[uint2(UV * Up.Length)];
			}
		}
	}
	return float4(0,0,0,0);
}