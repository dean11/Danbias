//Texture2D pos : register(t3);
//Texture2D normal : register(t4);
Texture1D<float3> rand : register(t8);
Texture1D<float3> sphere : register(t9);


float SSAOperPixel( int2 pixel)
{
	float3 rnd = rand[(pixel.x+pixel.y)%rand.Length.x];

	float3 nvec = normal[pixel].xyz;
	float3 tangent = normalize(rnd-nvec * dot(rnd, nvec));
	float3 biTangent = cross(nvec,tangent);

	float3x3 tbn; // = float3x3( nvec, tangent, normal);
	tbn[0] =nvec;
	tbn[1] =tangent;
	tbn[2] =biTangent;

	float occlusion = 0.0f;
	for(int i=0;i<sphere.Length.x;++i)
	{
		float3 sampled = mul( tbn, sphere[i].xyz);
		sample = sampled * SSAOSphereRadius + pos[pixel].xyz;

		//test acumulate
		float rangeCheck = abs(pos[pixel].z - sampled.z) < SSAOSphereRadius ? 1.0f : 0.0f;
		occlusion += (sampled.z <= pos[pixel].z ? 1.0 : 0.0) * rangeCheck;
	}

	return 1.0 - (occlusion / sphere.Length.x);
}