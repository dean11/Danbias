/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "OysterCollision3D.h"
#include "Utilities.h"
#include <limits>

using namespace Oyster::Math3D;

namespace Oyster { namespace Collision3D { namespace Utility
{

// PRIVATE HEADER ///////////////////////////////////////////////////
	namespace Private
	{
		const Float epsilon = (const Float)1e-20;

		// Float calculations can suffer roundingerrors. Which is where epsilon = 1e-20 comes into the picture
		inline bool EqualsZero( const Float &value )
		{ // by Dan Andersson
			return ::Utility::Value::Abs( value ) < epsilon;
		}

		// Float calculations can suffer roundingerrors. Which is where epsilon = 1e-20 comes into the picture
		inline bool NotEqualsZero( const Float &value )
		{ // by Dan Andersson
			return ::Utility::Value::Abs( value ) > epsilon;
		}

		// returns true if miss/reject
		bool BoxVsRayPerSlabCheck( const Float3 &axis, const Float &boundingOffset, const Float3 &deltaPos, const Float3 rayDirection, Float &tMin, Float &tMax )
		{ // by Dan Andersson
			Float e = axis.Dot( deltaPos ),
			f = axis.Dot( rayDirection );
			if( EqualsZero(f) ) // if axis is not parallell with ray
			{
				Float t1 = e + boundingOffset,
					  t2 = e - boundingOffset;
				t1 /= f; t2 /= f;
				if( t1 > t2 ) ::Utility::Element::Swap( t1, t2 );
				tMin = ::Utility::Value::Max( tMin, t1 );
				tMax = ::Utility::Value::Min( tMax, t2 );
				if( tMin > tMax ) return true;
				if( tMax < 0.0f ) return true;
			}
			else if( boundingOffset < -e ) return true;
			else if( boundingOffset < e ) return true;
		
			return false;
		}

		inline bool Contains( const Plane &container, const Float3 &pos )
		{ // by Dan Andersson
			return EqualsZero( container.normal.Dot( pos ) + container.phasing );
		}

		inline void Compare( Float &connectOffset, const Plane &plane, const Float3 &pos )
		{ // by Dan Andersson
			connectOffset = plane.normal.Dot(pos);
			connectOffset += plane.phasing;
		}

		void Compare( Float &boxExtend, Float &centerDistance, const Plane &plane, const BoxAxisAligned &box )
		{ // by Dan Andersson
			Float3 c = (box.maxVertex + box.minVertex) * 0.5f, // box.Center
				   h = (box.maxVertex - box.minVertex) * 0.5f; // box.halfSize
			boxExtend  = h.x * ::Utility::Value::Abs(plane.normal.x); // Box max extending towards plane
			boxExtend += h.y * ::Utility::Value::Abs(plane.normal.y);
			boxExtend += h.z * ::Utility::Value::Abs(plane.normal.z);
			centerDistance = c.Dot(plane.normal) + plane.phasing; // distance between box center and plane
		}

		void Compare( Float &boxExtend, Float &centerDistance, const Plane &plane, const Box &box )
		{ // by Dan Andersson
			boxExtend  = box.boundingOffset.x * ::Utility::Value::Abs(plane.normal.Dot(box.orientation.v[0].xyz)); // Box max extending towards plane
			boxExtend += box.boundingOffset.y * ::Utility::Value::Abs(plane.normal.Dot(box.orientation.v[1].xyz));
			boxExtend += box.boundingOffset.z * ::Utility::Value::Abs(plane.normal.Dot(box.orientation.v[2].xyz));

			centerDistance = box.orientation.v[3].xyz.Dot(plane.normal) + plane.phasing; // distance between box center and plane
		}

		bool FifteenAxisVsAlignedAxisOverlappingChecks( const Float3 &boundingOffsetA, const Float3 &boundingOffsetB, const Float4x4 &orientationB )
		{ // by Dan Andersson			
			Float4x4 absOrientationB;
			{
				Float4x4 tO = orientationB.GetTranspose();
				absOrientationB.v[0] = ::Utility::Value::Abs(tO.v[0]);
				if( absOrientationB.v[0].w > boundingOffsetA.x + boundingOffsetB.Dot(absOrientationB.v[0].xyz) ) return false;
				absOrientationB.v[1] = ::Utility::Value::Abs(tO.v[1]);
				if( absOrientationB.v[1].w > boundingOffsetA.y + boundingOffsetB.Dot(absOrientationB.v[1].xyz) ) return false;
				absOrientationB.v[2] = ::Utility::Value::Abs(tO.v[2]);
				if( absOrientationB.v[2].w > boundingOffsetA.z + boundingOffsetB.Dot(absOrientationB.v[2].xyz) ) return false;
			}

			absOrientationB.Transpose();
			if( ::Utility::Value::Abs(orientationB.v[3].Dot(orientationB.v[0])) > boundingOffsetB.x + boundingOffsetA.Dot(absOrientationB.v[0].xyz) ) return false;
			if( ::Utility::Value::Abs(orientationB.v[3].Dot(orientationB.v[1])) > boundingOffsetB.x + boundingOffsetA.Dot(absOrientationB.v[1].xyz) ) return false;
			if( ::Utility::Value::Abs(orientationB.v[3].Dot(orientationB.v[2])) > boundingOffsetB.x + boundingOffsetA.Dot(absOrientationB.v[2].xyz) ) return false;
		
			// ( 1,0,0 ) x orientationB.v[0].xyz:
			Float d = boundingOffsetA.y * absOrientationB.v[0].z;
			d += boundingOffsetA.z * absOrientationB.v[0].y;
			d += boundingOffsetB.y * absOrientationB.v[2].x;
			d += boundingOffsetB.z * absOrientationB.v[1].x;
			if( ::Utility::Value::Abs(orientationB.v[3].z*orientationB.v[0].y - orientationB.v[3].y*orientationB.v[0].z) > d ) return false;

			// ( 1,0,0 ) x orientationB.v[1].xyz:
			d  = boundingOffsetA.y * absOrientationB.v[1].z;
			d += boundingOffsetA.z * absOrientationB.v[1].y;
			d += boundingOffsetB.x * absOrientationB.v[2].x;
			d += boundingOffsetB.z * absOrientationB.v[0].x;
			if( ::Utility::Value::Abs(orientationB.v[3].z*orientationB.v[1].y - orientationB.v[3].y*orientationB.v[1].z) > d ) return false;

			// ( 1,0,0 ) x orientationB.v[2].xyz:
			d  = boundingOffsetA.y * absOrientationB.v[2].z;
			d += boundingOffsetA.z * absOrientationB.v[2].y;
			d += boundingOffsetB.x * absOrientationB.v[1].x;
			d += boundingOffsetB.y * absOrientationB.v[0].x;
			if( ::Utility::Value::Abs(orientationB.v[3].z*orientationB.v[2].y - orientationB.v[3].y*orientationB.v[2].z) > d ) return false;

			// ( 0,1,0 ) x orientationB.v[0].xyz:
			d  = boundingOffsetA.x * absOrientationB.v[0].z;
			d += boundingOffsetA.z * absOrientationB.v[0].x;
			d += boundingOffsetB.y * absOrientationB.v[2].y;
			d += boundingOffsetB.z * absOrientationB.v[1].y;
			if( ::Utility::Value::Abs(orientationB.v[3].x*orientationB.v[0].z - orientationB.v[3].z*orientationB.v[0].x) > d ) return false;
		
			// ( 0,1,0 ) x orientationB.v[1].xyz:
			d  = boundingOffsetA.x * absOrientationB.v[1].z;
			d += boundingOffsetA.z * absOrientationB.v[1].x;
			d += boundingOffsetB.x * absOrientationB.v[2].y;
			d += boundingOffsetB.z * absOrientationB.v[0].y;
			if( ::Utility::Value::Abs(orientationB.v[3].x*orientationB.v[1].z - orientationB.v[3].z*orientationB.v[1].x) > d ) return false;

			// ( 0,1,0 ) x orientationB.v[2].xyz:
			d  = boundingOffsetA.x * absOrientationB.v[2].z;
			d += boundingOffsetA.z * absOrientationB.v[2].x;
			d += boundingOffsetB.x * absOrientationB.v[1].y;
			d += boundingOffsetB.y * absOrientationB.v[0].y;
			if( ::Utility::Value::Abs(orientationB.v[3].x*orientationB.v[2].z - orientationB.v[3].z*orientationB.v[2].x) > d ) return false;

			// ( 0,0,1 ) x orientationB.v[0].xyz:
			d  = boundingOffsetA.x * absOrientationB.v[0].y;
			d += boundingOffsetA.y * absOrientationB.v[0].x;
			d += boundingOffsetB.y * absOrientationB.v[2].z;
			d += boundingOffsetB.z * absOrientationB.v[1].z;
			if( ::Utility::Value::Abs(orientationB.v[3].y*orientationB.v[0].x - orientationB.v[3].x*orientationB.v[0].y) > d ) return false;

			// ( 0,0,1 ) x orientationB.v[1].xyz:
			d  = boundingOffsetA.x * absOrientationB.v[1].y;
			d += boundingOffsetA.y * absOrientationB.v[1].x;
			d += boundingOffsetB.x * absOrientationB.v[2].z;
			d += boundingOffsetB.z * absOrientationB.v[0].z;
			if( ::Utility::Value::Abs(orientationB.v[3].y*orientationB.v[1].x - orientationB.v[3].x*orientationB.v[1].y) > d ) return false;

			// ( 0,0,1 ) x orientationB.v[2].xyz:
			d  = boundingOffsetA.x * absOrientationB.v[2].y;
			d += boundingOffsetA.y * absOrientationB.v[2].x;
			d += boundingOffsetB.x * absOrientationB.v[1].z;
			d += boundingOffsetB.y * absOrientationB.v[0].z;
			if( ::Utility::Value::Abs(orientationB.v[3].y*orientationB.v[2].x - orientationB.v[3].x*orientationB.v[2].y) > d ) return false;

			return true;
		}
	}

// PUBLIC BODY //////////////////////////////////////////////////////

	void Compare( Float &connectDistance, Float &connectOffsetSquared, const Ray &ray, const Point &point )
	{ // by Dan Andersson
		Float3 dP = point.center - ray.origin;

		connectDistance = dP.Dot( ray.direction );
		connectDistance /= ray.direction.Dot( ray.direction );

		dP -= ( connectDistance * ray.direction );
		connectOffsetSquared = dP.Dot( dP );
	}

	void Compare( Float &connectDistanceA, Float &connectDistanceB, Float &connectOffsetSquared, const Ray &rayA, const Ray &rayB )
	{ // by Dan Andersson
		Float3 dP = rayB.origin - rayA.origin;
		connectDistanceA = rayA.direction.Dot( dP );
		connectDistanceA /= rayA.direction.Dot( rayA.direction );

		dP *= -1.0f;
		connectDistanceB = rayB.direction.Dot( dP );
		connectDistanceB /= rayB.direction.Dot( rayB.direction );

		dP = rayA.direction * connectDistanceA;
		dP += rayA.origin;
		dP -= rayB.direction * connectDistanceB;
		dP -= rayB.origin;

		connectOffsetSquared = dP.Dot( dP );	
	}

	void Compare( Float &connectOffset, const Plane &plane, const Point &point )
	{ // by Dan Andersson
		Private::Compare( connectOffset, plane, point.center );
	}

	bool Intersect( const Point &pointA, const Point &pointB )
	{ // by Fredrick Johansson
		if (pointA.center.x != pointB.center.x) return false;
		if (pointA.center.y != pointB.center.y) return false;
		if (pointA.center.z != pointB.center.z) return false;
		return true; // Passed all tests, is in same position
	}

	bool Intersect( const Ray &ray, const Point &point, Float &connectDistance )
	{ // by Dan Andersson
		Float connectOffsetSquared;
		Compare( connectDistance, connectOffsetSquared, ray, point );

		if( Private::EqualsZero(connectOffsetSquared) )
		{
			connectOffsetSquared = 0.0f;
			return true;
		}

		connectDistance = 0.0f;
		return false;
	}

	bool Intersect( const Ray &rayA, const Ray &rayB, Float &connectDistanceA, Float &connectDistanceB )
	{ // by Dan Andersson
		Float connectOffsetSquared;
		Compare( connectDistanceA, connectDistanceB, connectOffsetSquared, rayA, rayB );

		if( Private::EqualsZero(connectOffsetSquared) )
		{
			connectOffsetSquared = 0.0f;
			return true;
		}

		connectDistanceA = connectDistanceB = 0.0f;
		return false;
	}

	bool Intersect( const Sphere &sphere, const Point &point )
	{ // by Dan Andersson
		Float3 dP = point.center - sphere.center;
		if( dP.Dot(dP) > (sphere.radius * sphere.radius) )
			return false;
		return true;
	}

	bool Intersect( const Sphere &sphere, const Ray &ray, Float &connectDistance )
	{// by Dan Andersson
		Float3 dP = sphere.center - ray.origin;
		Float s = dP.Dot( ray.direction ),
			  dSquared = dP.Dot( dP ),
			  rSquared = sphere.radius * sphere.radius;

		if( dSquared <= rSquared ) { connectDistance = 0.0f; return true; }
		else if( s < 0.0f ) { connectDistance = 0.0f; return false; }

		Float mSquared = dSquared - (s*s);
		if( mSquared > rSquared ) { connectDistance = 0.0f; return false; }
		
		Float q = ::std::sqrt( rSquared - mSquared );
		if( dSquared > rSquared ) connectDistance = s - q;
		else connectDistance = s + q;

		return true;
	}

	bool Intersect( const Sphere &sphereA, const Sphere &sphereB )
	{ // by Fredrick Johansson
		Float3 C = sphereA.center;
		C -= sphereB.center;
		Float r = (sphereA.radius + sphereB.radius);

		if (r*r >= C.Dot(C))
		{
			return true; // Intersect detected!
		}

		return false;
	}

	bool Intersect( const Plane &plane, const Point &point )
	{ // by Dan Andersson
		Float connectOffset;
		Private::Compare( connectOffset, plane, point.center );
		return Private::EqualsZero(connectOffset);
	}

	bool Intersect( const Plane &plane, const Ray &ray, Float &connectDistance )
	{ // by Dan Andersson
		Float c = plane.normal.Dot(ray.direction);
		if( Private::EqualsZero(c) )
		{ // ray is parallell with the plane. (ray direction orthogonal with the planar normal)
			connectDistance = 0.0f;
			return Contains( plane, ray.origin );
		}

		connectDistance = -plane.phasing;
		connectDistance -= plane.normal.Dot( ray.origin );
		connectDistance /= c;

		if( connectDistance > 0.0f )
			return true;

		connectDistance = 0.0f;
		return false;
	}

	bool Intersect( const Plane &plane, const Sphere &sphere )
	{ // by Dan Andersson
		Float connectOffset;
		Private::Compare( connectOffset, plane, sphere.center );
		return (connectOffset <= sphere.radius);
	}

	bool Intersect( const Plane &planeA, const Plane &planeB )
	{ // by Dan Andersson
		if( planeA.normal == planeB.normal ) // they are parallell
			return (planeA.phasing == planeB.phasing);
		else if( planeA.normal == -planeB.normal )  // they are still parallell
			return (planeA.phasing == -planeB.phasing);
		return true; // none parallell planes ALWAYS intersects somewhere
	}

	bool Intersect( const BoxAxisAligned &box, const Point &point )
	{ // by Dan Andersson
		if( point.center.x < box.minVertex.x ) return false;
		if( point.center.x > box.maxVertex.x ) return false;
		if( point.center.y < box.minVertex.y ) return false;
		if( point.center.y > box.maxVertex.y ) return false;
		if( point.center.z < box.minVertex.z ) return false;
		if( point.center.z > box.maxVertex.z ) return false;
		return true;
	}

	bool Intersect( const BoxAxisAligned &box, const Ray &ray, Float &connectDistance )
	{ // by Dan Andersson
		Float tMin = ::std::numeric_limits<Float>::max(),
			  tMax = -tMin; // initiating to extremevalues

		Float3 boundingOffset = ((box.maxVertex - box.minVertex) * 0.5f),
			   dP = ((box.maxVertex + box.minVertex) * 0.5f) - ray.origin;
		if( Private::BoxVsRayPerSlabCheck( Float3::standard_unit_x, boundingOffset.x, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		if( Private::BoxVsRayPerSlabCheck( Float3::standard_unit_y, boundingOffset.y, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		if( Private::BoxVsRayPerSlabCheck( Float3::standard_unit_z, boundingOffset.z, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		
		if( tMin > 0.0f ) connectDistance = tMin;
		else connectDistance = tMax;
		return true;
	}

	bool Intersect( const BoxAxisAligned &box, const Sphere &sphere )
	{ // by Dan Andersson
		Float3 e = ::Utility::Value::Max( box.minVertex - sphere.center, Float3::null );
		e += ::Utility::Value::Max( sphere.center - box.maxVertex, Float3::null );

		if( e.Dot(e) > (sphere.radius * sphere.radius) ) return false;
		return true;
	}

	bool Intersect( const BoxAxisAligned &box, const Plane &plane )
	{ // by Dan Andersson
		Float e, d;
		Private::Compare( e, d, plane, box );
		if( d - e > 0.0f ) return false; // is beneath
		if( d + e < 0.0f ) return false; // is above
		return true;
	}

//	bool Intersect( const BoxAxisAligned &box, const Triangle &triangle )
//	{ return false; /* TODO:  */ }

	bool Intersect( const BoxAxisAligned &boxA, const BoxAxisAligned &boxB )
	{ // by Dan Andersson
		if( boxA.maxVertex.x < boxB.minVertex.x ) return false;
		if( boxA.minVertex.x > boxB.maxVertex.x ) return false;
		if( boxA.maxVertex.y < boxB.minVertex.y ) return false;
		if( boxA.minVertex.y > boxB.maxVertex.y ) return false;
		if( boxA.maxVertex.z < boxB.minVertex.z ) return false;
		if( boxA.minVertex.z > boxB.maxVertex.z ) return false;
		return true;
	}

	bool Intersect( const Box &box, const Point &point )
	{ // by Dan Andersson
		Float3 dPos = point.center - box.orientation.v[3].xyz;

		Float coordinate = dPos.Dot( box.orientation.v[0].xyz );
		if( coordinate > box.boundingOffset.x ) return false;
		if( coordinate < -box.boundingOffset.x ) return false;

		coordinate = dPos.Dot( box.orientation.v[1].xyz );
		if( coordinate > box.boundingOffset.y ) return false;
		if( coordinate < -box.boundingOffset.y ) return false;

		coordinate = dPos.Dot( box.orientation.v[2].xyz );
		if( coordinate > box.boundingOffset.z ) return false;
		if( coordinate < -box.boundingOffset.z ) return false;

		return true;
	}

	bool Intersect( const Box &box, const Ray &ray, Float &connectDistance )
	{ // by Dan Andersson
		Float tMin = ::std::numeric_limits<Float>::max(),
			  tMax = -tMin; // initiating to extremevalues

		Float3 dP = box.center - ray.origin;
		if( Private::BoxVsRayPerSlabCheck( box.xAxis, box.boundingOffset.x, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		if( Private::BoxVsRayPerSlabCheck( box.yAxis, box.boundingOffset.y, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		if( Private::BoxVsRayPerSlabCheck( box.zAxis, box.boundingOffset.z, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		
		if( tMin > 0.0f ) connectDistance = tMin;
		else connectDistance = tMax;
		return true;
	}

	bool Intersect( const Box &box, const Sphere &sphere )
	{ // by Dan Andersson
		Float3 e = sphere.center - box.orientation.v[3].xyz,
			   centerL = Float3( e.Dot(box.orientation.v[0].xyz), e.Dot(box.orientation.v[1].xyz), e.Dot(box.orientation.v[2].xyz) );
		
		e  = ::Utility::Value::Max( (box.boundingOffset + centerL)*=-1.0f, Float3::null );
		e += ::Utility::Value::Max( centerL - box.boundingOffset, Float3::null );
		
		if( e.Dot(e) > (sphere.radius * sphere.radius) ) return false;
		return true;
	}

	bool Intersect( const Box &box, const Plane &plane )
	{// by Dan Andersson
		Float e, d;
		Private::Compare( e, d, plane, box );
		if( d - e > 0.0f ) return false; // is beneath
		if( d + e < 0.0f ) return false; // is above
		return true;	
	}

	bool Intersect( const Box &boxA, const BoxAxisAligned &boxB )
	{ // by Dan Andersson
		Float3 alignedOffsetBoundaries = boxB.maxVertex - boxB.minVertex;
		Float4x4 translated = boxA.orientation;
		translated.v[3].xyz -= boxB.minVertex;
		translated.v[3].xyz += alignedOffsetBoundaries * 0.5f;
		alignedOffsetBoundaries = ::Utility::Value::Abs(alignedOffsetBoundaries);
		return Private::FifteenAxisVsAlignedAxisOverlappingChecks( alignedOffsetBoundaries, boxA.boundingOffset, translated );
	}

	bool Intersect( const Box &boxA, const Box &boxB )
	{ // by Dan Andersson
		Float4x4 M;
		InverseOrientationMatrix( boxA.orientation, M );
		TransformMatrix( M, boxB.orientation, M ); /// TODO: not verified
		return Private::FifteenAxisVsAlignedAxisOverlappingChecks( boxA.boundingOffset, boxB.boundingOffset, M );
	}

	bool Intersect( const Frustrum &frustrum, const Point &point )
	{ // by Dan Andersson
		Float connectOffset;
		
		Private::Compare( connectOffset, frustrum.leftPlane, point.center );
		if( connectOffset < 0.0f ) return false;

		Private::Compare( connectOffset, frustrum.rightPlane, point.center );
		if( connectOffset < 0.0f ) return false;

		Private::Compare( connectOffset, frustrum.bottomPlane, point.center );
		if( connectOffset < 0.0f) return false;

		Private::Compare( connectOffset, frustrum.topPlane, point.center );
		if( connectOffset < 0.0f) return false;

		Private::Compare( connectOffset, frustrum.nearPlane, point.center );
		if( connectOffset < 0.0f ) return false;

		Private::Compare( connectOffset, frustrum.farPlane, point.center );
		if( connectOffset < 0.0f ) return false;

		return true;
	}

	bool Intersect( const Frustrum &frustrum, const Ray &ray, Float &connectDistance )
	{ // by Dan Andersson
		bool intersected = false;
		Float distance = 0.0f;
		connectDistance = ::std::numeric_limits<Float>::max();

		if( Intersect(frustrum.leftPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = ::Utility::Value::Min( connectDistance, distance );
		}

		if( Intersect(frustrum.rightPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = ::Utility::Value::Min( connectDistance, distance );
		}

		if( Intersect(frustrum.bottomPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = ::Utility::Value::Min( connectDistance, distance );
		}

		if( Intersect(frustrum.topPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = ::Utility::Value::Min( connectDistance, distance );
		}

		if( Intersect(frustrum.nearPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = ::Utility::Value::Min( connectDistance, distance );
		}

		if( Intersect(frustrum.farPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = ::Utility::Value::Min( connectDistance, distance );
		}

		if( intersected ) return true;

		connectDistance = 0.0f;
		return false;
	}

	bool Intersect( const Frustrum &frustrum, const Sphere &sphere )
	{ // by Dan Andersson
		Float connectOffset;

		Private::Compare( connectOffset, frustrum.leftPlane, sphere.center );
		if( connectOffset < -sphere.radius  ) return false;

		Private::Compare( connectOffset, frustrum.rightPlane, sphere.center );
		if( connectOffset < -sphere.radius ) return false;

		Private::Compare( connectOffset, frustrum.bottomPlane, sphere.center );
		if( connectOffset < -sphere.radius ) return false;

		Private::Compare( connectOffset, frustrum.topPlane, sphere.center );
		if( connectOffset < -sphere.radius ) return false;

		Private::Compare( connectOffset, frustrum.nearPlane, sphere.center );
		if( connectOffset < -sphere.radius ) return false;

		Private::Compare( connectOffset, frustrum.farPlane, sphere.center );
		if( connectOffset < -sphere.radius ) return false;

		return true;
	}

	bool Intersect( const Frustrum &frustrum, const Plane &plane )
	{
		return false; // TODO: 
	}

//	bool Intersect( const Frustrum &frustrum, const Triangle &triangle, ? );

	bool Intersect( const Frustrum &frustrum, const BoxAxisAligned &box )
	{ // by Dan Andersson
		Float e, d;

		Private::Compare( e, d, frustrum.leftPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.rightPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.bottomPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.topPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.nearPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.farPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		return true;
	}

	bool Intersect( const Frustrum &frustrum, const Box &box )
	{ // by Dan Andersson
		Float e, d;
		
		Private::Compare( e, d, frustrum.leftPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.rightPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.bottomPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.topPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.nearPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		Private::Compare( e, d, frustrum.farPlane, box );
		if( d - e > 0.0f ) return false; // is beneath

		return true;	
	}

	bool Intersect( const Frustrum &frustrumA, const Frustrum &frustrumB )
	{
		return false; // TODO: 
	}

	bool Contains( const Ray &container, const Ray &ray )
	{
		return false; /*TODO: */
	}

	bool Contains( const Sphere &sphereA, const Sphere &sphereB )
	{ // by Fredrick Johansson
		// Check if SphereB is larger than sphereA
		if (sphereA.radius < sphereB.radius)
		{
			return false;	// Is impossible, yes
		}

		// Calc distance from center to center
		Float3 d		= sphereB.center - sphereA.center; 
		Float deltaR	= sphereA.radius - sphereB.radius;

		// Check if contained
		if (d.Dot(d) <= (deltaR*deltaR))
		{
			return true;
		}

		// Not contained
		return false;
	}

	bool Contains( const Plane &container, const Point &point )
	{ // by Dan Andersson
		return Private::Contains( container, point.center );
	}

	bool Contains( const Plane &container, const Ray &ray )
	{ // by Dan Andersson
		if( Private::NotEqualsZero(container.normal.Dot(ray.direction)) ) return false;
		return Contains( container, ray.origin );
	}

	bool Contains( const Plane &container, const Plane &plane )
	{ // by Dan Andersson
		if( container.phasing == plane.phasing )
			return container.normal == plane.normal;
		if( container.phasing == -plane.phasing )
			return container.normal == -plane.normal;
		return false;
	}
} } }