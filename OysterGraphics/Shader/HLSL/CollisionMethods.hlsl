#ifndef COLLISIONMETHODS_HLSL
#define COLLISIONMETHODS_HLSL

struct Sphere
{
	float3 center;
	float radius;
};

struct Plane
{
	float3 normal;
	float phasing;
};

struct Frustrum
{
	Plane leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane;
};

bool intersects( uniform Frustrum f, uniform Sphere s )
{
	float connectOffset;

	connectOffset = dot( f.leftPlane.normal, s.center );
	connectOffset += f.leftPlane.phasing;
	if( connectOffset < -s.radius ) return false;

	connectOffset = dot( f.rightPlane.normal, s.center );
	connectOffset += f.rightPlane.phasing;
	if( connectOffset < -s.radius ) return false;

	connectOffset = dot( f.bottomPlane.normal, s.center );
	connectOffset += f.bottomPlane.phasing;
	if( connectOffset < -s.radius ) return false;

	connectOffset = dot( f.topPlane.normal, s.center );
	connectOffset += f.topPlane.phasing;
	if( connectOffset < -s.radius ) return false;

	connectOffset = dot( f.nearPlane.normal, s.center );
	connectOffset += f.nearPlane.phasing;
	if( connectOffset < -s.radius ) return false;

	connectOffset = dot( f.farPlane.normal, s.center );
	connectOffset += f.farPlane.phasing;
	if( connectOffset < -s.radius ) return false;

	return true;
}

#endif