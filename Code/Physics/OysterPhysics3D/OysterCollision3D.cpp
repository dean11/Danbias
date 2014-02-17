/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "OysterCollision3D.h"
#include "Utilities.h"
#include <limits>

using namespace ::Oyster::Math3D;
using namespace ::Utility::Value;

namespace Oyster { namespace Collision3D { namespace Utility
{

// PRIVATE HEADER ///////////////////////////////////////////////////
	namespace Private
	{
		const Float epsilon = (const Float)1e-20;

		// Float calculations can suffer roundingerrors. Which is where epsilon = 1e-20 comes into the picture
		inline bool EqualsZero( const Float &value )
		{ // by Dan Andersson
			return Abs( value ) < epsilon;
		}

		// Float calculations can suffer roundingerrors. Which is where epsilon = 1e-20 comes into the picture
		inline bool NotEqualsZero( const Float &value )
		{ // by Dan Andersson
			return Abs( value ) > epsilon;
		}

		// returns true if miss/reject
		bool BoxVsRayPerSlabCheck( const Float4 &axis, const Float &boundingOffset, const Float4 &deltaPos, const Float4 rayDirection, Float &tMin, Float &tMax )
		{ // by Dan Andersson
			Float e = axis.Dot( deltaPos ),
				  f = axis.Dot( rayDirection );
			if( EqualsZero(f) ) // if axis is not parallell with ray
			{
				Float t1 = e + boundingOffset,
					  t2 = e - boundingOffset;
				t1 /= f; t2 /= f;
				if( t1 > t2 ) ::Utility::Element::Swap( t1, t2 );
				tMin = Max( tMin, t1 );
				tMax = Min( tMax, t2 );
				if( tMin > tMax ) return true;
				if( tMax < 0.0f ) return true;
			}
			else if( boundingOffset < -e ) return true;
			else if( boundingOffset < e ) return true;
		
			return false;
		}

		inline bool Contains( const Plane &container, const Float4 &pos )
		{ // by Dan Andersson
			return EqualsZero( container.normal.Dot( pos ) + container.phasing );
		}

		inline void Compare( Float &connectOffset, const Plane &plane, const Float4 &pos )
		{ // by Dan Andersson
			connectOffset = plane.normal.Dot(pos);
			connectOffset += plane.phasing;
		}

		void Compare( Float &boxExtend, Float &centerDistance, const Plane &plane, const BoxAxisAligned &box )
		{ // by Dan Andersson
			Float4 c = (box.maxVertex + box.minVertex) * 0.5f, // box.Center
				   h = (box.maxVertex - box.minVertex) * 0.5f; // box.halfSize
			boxExtend  = h.x * Abs(plane.normal.x); // Box max extending towards plane
			boxExtend += h.y * Abs(plane.normal.y);
			boxExtend += h.z * Abs(plane.normal.z);
			centerDistance = c.Dot(plane.normal) + plane.phasing; // distance between box center and plane
		}

		void Compare( Float &boxExtend, Float &centerDistance, const Plane &plane, const Box &box )
		{ // by Dan Andersson
			boxExtend  = box.boundingOffset.x * Abs(plane.normal.Dot(box.xAxis)); // Box max extending towards plane
			boxExtend += box.boundingOffset.y * Abs(plane.normal.Dot(box.yAxis));
			boxExtend += box.boundingOffset.z * Abs(plane.normal.Dot(box.zAxis));

			centerDistance = box.center.Dot(plane.normal) + plane.phasing; // distance between box center and plane
		}

		bool SeperatingAxisTest_AxisAlignedVsTransformedBox( const Float4 &boundingOffsetA, const Float4 &boundingOffsetB, const Float4x4 &rotationB, const Float4 &worldOffset )
		{ // by Dan Andersson

			/*****************************************************************
			 * Uses the Seperating Axis Theorem
			 * if( |t dot s| > hA dot |s * RA| + hB dot |s * RB| ) then not intersecting
			 *     |t dot s| > hA dot |s| + hB dot |s * RB| .. as RA = I
			 *
			 * t: objectB's offset from objectA														[worldOffset]
			 * s: current comparison axis
			 * hA: boundingReach vector of objectA. Only absolute values is assumed.				[boundingOffsetA]
			 * hB: boundingReach vector of objectB. Only absolute values is assumed.				[boundingOffsetB]
			 * RA: rotation matrix of objectA. Is identity matrix here, thus omitted.
			 * RB: rotation matrix of objectB. Is transformed into objectA's view at this point.	[rotationB]
			 *
			 * Note: s * RB = (RB^T * s)^T = (RB^-1 * s)^T .... vector == vector^T
			 *****************************************************************/

			Float4x4 absRotationB = Abs(rotationB);
			Float3 absWorldOffset = Abs(worldOffset); // |t|: [absWorldOffset]

			// s = { 1, 0, 0 }	[ RA.v[0] ]
			if( absWorldOffset.x > boundingOffsetA.x + boundingOffsetB.Dot(Float4(absRotationB.v[0].x, absRotationB.v[1].x, absRotationB.v[2].x, 0.0f)) )
			{ // |t dot s| > hA dot |s| + hB dot |s * RB| -->> t.x > hA.x + hB dot |{RB.v[0].x, RB.v[1].x, RB.v[2].x}| 
				return false;
			}

			// s = { 0, 1, 0 }	[ RA.v[1] ]
			if( absWorldOffset.y > boundingOffsetA.y + boundingOffsetB.Dot(Float4(absRotationB.v[0].y, absRotationB.v[1].y, absRotationB.v[2].y, 0.0f)) )
			{ // t.y > hA.y + hB dot |{RB.v[0].y, RB.v[1].y, RB.v[2].y}|
				return false;
			}

			// s = { 0, 0, 1 }	[ RA.v[2] ]
			if( absWorldOffset.z > boundingOffsetA.z + boundingOffsetB.Dot(Float4(absRotationB.v[0].z, absRotationB.v[1].z, absRotationB.v[2].z, 0.0f)) )
			{ // t.z > hA.z + hB dot |{RB.v[0].z, RB.v[1].z, RB.v[2].z}|
				return false;
			}

			// s = RB.v[0].xyz
			if( Abs(worldOffset.Dot(rotationB.v[0])) > boundingOffsetA.Dot(absRotationB.v[0]) + boundingOffsetB.x )
			{ // |t dot s| > hA dot |s| + hB dot |s * RB| -->> |t dot s| > hA dot |s| + hB dot |{1, 0, 0}| -->> |t dot s| > hA dot |s| + hB.x
				return false;
			}

			// s = RB.v[1].xyz
			if( Abs(worldOffset.Dot(rotationB.v[1])) > boundingOffsetA.Dot(absRotationB.v[1]) + boundingOffsetB.y )
			{ // |t dot s| > hA dot |s| + hB.y
				return false;
			}

			// s = RB.v[2].xyz
			if( Abs(worldOffset.Dot(rotationB.v[2])) > boundingOffsetA.Dot(absRotationB.v[2]) + boundingOffsetB.z )
			{ // |t dot s| > hA dot |s| + hB.z
				return false;
			}

			// s = ( 1,0,0 ) x rotationB.v[0].xyz:
			Float d = boundingOffsetA.y * absRotationB.v[0].z;
			d += boundingOffsetA.z * absRotationB.v[0].y;
			d += boundingOffsetB.y * absRotationB.v[2].x;
			d += boundingOffsetB.z * absRotationB.v[1].x;
			if( Abs(worldOffset.z*rotationB.v[0].y - worldOffset.y*rotationB.v[0].z) > d ) return false;

			// s = ( 1,0,0 ) x rotationB.v[1].xyz:
			d  = boundingOffsetA.y * absRotationB.v[1].z;
			d += boundingOffsetA.z * absRotationB.v[1].y;
			d += boundingOffsetB.x * absRotationB.v[2].x;
			d += boundingOffsetB.z * absRotationB.v[0].x;
			if( Abs(worldOffset.z*rotationB.v[1].y - worldOffset.y*rotationB.v[1].z) > d ) return false;

			// s = ( 1,0,0 ) x rotationB.v[2].xyz:
			d  = boundingOffsetA.y * absRotationB.v[2].z;
			d += boundingOffsetA.z * absRotationB.v[2].y;
			d += boundingOffsetB.x * absRotationB.v[1].x;
			d += boundingOffsetB.y * absRotationB.v[0].x;
			if( Abs(worldOffset.z*rotationB.v[2].y - worldOffset.y*rotationB.v[2].z) > d ) return false;

			// s = ( 0,1,0 ) x rotationB.v[0].xyz:
			d  = boundingOffsetA.x * absRotationB.v[0].z;
			d += boundingOffsetA.z * absRotationB.v[0].x;
			d += boundingOffsetB.y * absRotationB.v[2].y;
			d += boundingOffsetB.z * absRotationB.v[1].y;
			if( Abs(worldOffset.x*rotationB.v[0].z - worldOffset.z*rotationB.v[0].x) > d ) return false;
		
			// s = ( 0,1,0 ) x rotationB.v[1].xyz:
			d  = boundingOffsetA.x * absRotationB.v[1].z;
			d += boundingOffsetA.z * absRotationB.v[1].x;
			d += boundingOffsetB.x * absRotationB.v[2].y;
			d += boundingOffsetB.z * absRotationB.v[0].y;
			if( Abs(worldOffset.x*rotationB.v[1].z - worldOffset.z*rotationB.v[1].x) > d ) return false;

			// s = ( 0,1,0 ) x rotationB.v[2].xyz:
			d  = boundingOffsetA.x * absRotationB.v[2].z;
			d += boundingOffsetA.z * absRotationB.v[2].x;
			d += boundingOffsetB.x * absRotationB.v[1].y;
			d += boundingOffsetB.y * absRotationB.v[0].y;
			if( Abs(worldOffset.x*rotationB.v[2].z - worldOffset.z*rotationB.v[2].x) > d ) return false;

			// s = ( 0,0,1 ) x rotationB.v[0].xyz:
			d  = boundingOffsetA.x * absRotationB.v[0].y;
			d += boundingOffsetA.y * absRotationB.v[0].x;
			d += boundingOffsetB.y * absRotationB.v[2].z;
			d += boundingOffsetB.z * absRotationB.v[1].z;
			if( Abs(worldOffset.y*rotationB.v[0].x - worldOffset.x*rotationB.v[0].y) > d ) return false;

			// s = ( 0,0,1 ) x rotationB.v[1].xyz:
			d  = boundingOffsetA.x * absRotationB.v[1].y;
			d += boundingOffsetA.y * absRotationB.v[1].x;
			d += boundingOffsetB.x * absRotationB.v[2].z;
			d += boundingOffsetB.z * absRotationB.v[0].z;
			if( Abs(worldOffset.y*rotationB.v[1].x - worldOffset.x*rotationB.v[1].y) > d ) return false;

			// s = ( 0,0,1 ) x rotationB.v[2].xyz:
			d  = boundingOffsetA.x * absRotationB.v[2].y;
			d += boundingOffsetA.y * absRotationB.v[2].x;
			d += boundingOffsetB.x * absRotationB.v[1].z;
			d += boundingOffsetB.y * absRotationB.v[0].z;
			if( Abs(worldOffset.y*rotationB.v[2].x - worldOffset.x*rotationB.v[2].y) > d ) return false;

			return true;
		}

		bool SeperatingAxisTest_AxisAlignedVsTransformedBox( const Float4 &boundingOffsetA, const Float4 &boundingOffsetB, const Float4x4 &rotationB, const Float4 &worldOffset, Float4 &localPointOfContact )
		{ // by Dan Andersson

			/*****************************************************************
			 * Uses the Seperating Axis Theorem
			 * if( |t dot s| > hA dot |s * RA| + hB dot |s * RB| ) then not intersecting
			 *     |t dot s| > hA dot |s| + hB dot |s * RB| .. as RA = I
			 *
			 * t: objectB's offset from objectA														[worldOffset]
			 * s: current comparison axis
			 * hA: boundingReach vector of objectA. Only absolute values is assumed.				[boundingOffsetA]
			 * hB: boundingReach vector of objectB. Only absolute values is assumed.				[boundingOffsetB]
			 * RA: rotation matrix of objectA. Is identity matrix here, thus omitted.
			 * RB: rotation matrix of objectB. Is transformed into objectA's view at this point.	[rotationB]
			 *
			 * Note: s * RB = (RB^T * s)^T = (RB^-1 * s)^T .... vector == vector^T
			 *****************************************************************/

			/*****************************************************************
			 * Distance Alghorithm based on .. something Dan came up with
			 * pi = si * ( (t dot si) * (hA dot |si|) / (hA dot |si| + hB dot |si * RB|) )
			 * p = estimated point of contact
			 *   = ( p1 + p2 + ... + p5 + p6 ) / 2
			 *****************************************************************/

			const Float4 &t =  worldOffset,
						 &hA = boundingOffsetA,
						 &hB = boundingOffsetB;
			Float4  s = Float4::standard_unit_x;
				   
			Float centerSeperation = t.Dot(s),
				  eA = hA.Dot( Abs(s) ),
				  edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}
			localPointOfContact = s * ( centerSeperation * eA / edgeSeperation );

			s = Float4::standard_unit_y;
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}
			localPointOfContact += s * ( centerSeperation * eA / edgeSeperation );

			s = Float4::standard_unit_z;
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}
			localPointOfContact += s * ( centerSeperation * eA / edgeSeperation );

			s = rotationB.v[0];
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}
			localPointOfContact += s * ( centerSeperation * eA / edgeSeperation );

			s = rotationB.v[1];
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}
			localPointOfContact += s * ( centerSeperation * eA / edgeSeperation );

			s = rotationB.v[2];
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}
			localPointOfContact += s * ( centerSeperation * eA / edgeSeperation ); // enough point of contact data gathered for approximative result.

			s = Float4( Float3::standard_unit_x.Cross(rotationB.v[0].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			s = Float4( Float3::standard_unit_x.Cross(rotationB.v[1].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			s = Float4( Float3::standard_unit_x.Cross(rotationB.v[2].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			s = Float4( Float3::standard_unit_y.Cross(rotationB.v[0].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			s = Float4( Float3::standard_unit_y.Cross(rotationB.v[1].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			s = Float4( Float3::standard_unit_y.Cross(rotationB.v[2].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			s = Float4( Float3::standard_unit_z.Cross(rotationB.v[0].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			s = Float4( Float3::standard_unit_z.Cross(rotationB.v[1].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			s = Float4( Float3::standard_unit_z.Cross(rotationB.v[2].xyz), 0.0f );
			centerSeperation = t.Dot(s);
			eA = hA.Dot( Abs(s) );
			edgeSeperation = eA + hB.Dot( Abs(s * rotationB) );
			if( Abs(centerSeperation) > edgeSeperation )
			{ // no intersection
				return false;
			}

			localPointOfContact *= 0.5f;
			return true;
		}
	}

// PUBLIC BODY //////////////////////////////////////////////////////

	void Compare( Float &connectDistance, Float &connectOffsetSquared, const Ray &ray, const Point &point )
	{ // by Dan Andersson
		Float4 dP = point.center - ray.origin;

		connectDistance = dP.Dot( ray.direction );
		connectDistance /= ray.direction.Dot( ray.direction );

		dP -= ( connectDistance * ray.direction );
		connectOffsetSquared = dP.Dot( dP );
	}

	void Compare( Float &connectDistanceA, Float &connectDistanceB, Float &connectOffsetSquared, const Ray &rayA, const Ray &rayB )
	{ // by Dan Andersson
		Float4 dP = rayB.origin - rayA.origin;
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

	bool Intersect( const Point &pointA, const Point &pointB, ::Oyster::Math::Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
		return false;
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

	bool Intersect( const Ray &ray, const Point &point, ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
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

	bool Intersect( const Ray &rayA, const Ray &rayB, ::Oyster::Math::Float &connectDistanceA, ::Oyster::Math::Float &connectDistanceB, ::Oyster::Math::Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
		return false;
	}

	bool Intersect( const Sphere &sphere, const Point &point )
	{ // by Dan Andersson
		Float3 dP = point.center - sphere.center;
		if( dP.Dot(dP) > (sphere.radius * sphere.radius) )
			return false;
		return true;
	}

	bool Intersect( const Sphere &sphere, const Point &point, ::Oyster::Math::Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
		return false;
	}

	bool Intersect( const Sphere &sphere, const Ray &ray, Float &connectDistance )
	{// by Dan Andersson
		Float4 dP = sphere.center - ray.origin;
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

	bool Intersect( const Sphere &sphere, const Ray &ray, ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
		return false;
	}

	bool Intersect( const Sphere &sphereA, const Sphere &sphereB )
	{ // by Fredrick Johansson
		Float4 C = sphereA.center;
		C -= sphereB.center;
		Float r = (sphereA.radius + sphereB.radius);
		Float dotprod = C.Dot(C);
		if (r*r >= C.Dot(C))
		{
			return true; // Intersect detected!
		}

		return false;
	}
	
	bool Intersect( const Sphere &sphereA, const Sphere &sphereB, ::Oyster::Math::Float4 &worldPointOfContact )
	{ // by Robin Engman
		Float4 C = sphereA.center;
		C -= sphereB.center;
		Float r = sphereA.radius + sphereB.radius;

		if ( r*r >= C.Dot(C) )
		{
			Float distance;
			Ray ray(sphereB.center, C.Normalize());

			Intersect( sphereA, ray, distance );

			worldPointOfContact = ray.origin + ray.direction*distance;

			return true;
		}

		return false;
	}

	bool Intersect( const Plane &plane, const Point &point )
	{ // by Dan Andersson
		Float connectOffset;
		Private::Compare( connectOffset, plane, point.center );
		return Private::EqualsZero(connectOffset);
	}

	bool Intersect( const Plane &plane, const Point &point, Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
		return false;
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

	bool Intersect( const Plane &plane, const Ray &ray, Float &connectDistance, Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
		return false;
	}

	bool Intersect( const Plane &plane, const Sphere &sphere )
	{ // by Dan Andersson
		Float connectOffset;
		Private::Compare( connectOffset, plane, sphere.center );
		return (connectOffset <= sphere.radius);
	}

	bool Intersect( const Plane &plane, const Sphere &sphere, Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
		return false;
	}

	bool Intersect( const Plane &planeA, const Plane &planeB )
	{ // by Dan Andersson
		if( planeA.normal == planeB.normal ) // they are parallell
			return (planeA.phasing == planeB.phasing);
		else if( planeA.normal == -planeB.normal )  // they are still parallell
			return (planeA.phasing == -planeB.phasing);
		return true; // none parallell planes ALWAYS intersects somewhere
	}

	bool Intersect( const Plane &planeA, const Plane &planeB, Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement Stub
		return false;
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

	bool Intersect( const BoxAxisAligned &box, const Point &point, Float4 &worldPointOfContact )
	{ // by Dan Andersson
		if( Intersect(box, point) )
		{
			worldPointOfContact = point.center;
			return true;
		}
		return false;
	}

	bool Intersect( const BoxAxisAligned &box, const Ray &ray, Float &connectDistance )
	{ // by Dan Andersson
		Float tMin = ::std::numeric_limits<Float>::max(),
			  tMax = -tMin; // initiating to extremevalues

		Float4 boundingOffset = ((box.maxVertex - box.minVertex) * 0.5f),
			   dP = ((box.maxVertex + box.minVertex) * 0.5f) - ray.origin;
		if( Private::BoxVsRayPerSlabCheck( Float4::standard_unit_x, boundingOffset.x, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		if( Private::BoxVsRayPerSlabCheck( Float4::standard_unit_y, boundingOffset.y, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		if( Private::BoxVsRayPerSlabCheck( Float4::standard_unit_z, boundingOffset.z, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		
		if( tMin > 0.0f ) connectDistance = tMin;
		else connectDistance = tMax;
		return true;
	}

	bool Intersect( const BoxAxisAligned &box, const Ray &ray, Float &connectDistance, Float4 &worldPointOfContact )
	{ // by Dan Andersson
		if( Intersect(box, ray, connectDistance) )
		{
			worldPointOfContact = ray.origin + ray.direction * connectDistance;
			return true;
		}
		return false;
	}

	bool Intersect( const BoxAxisAligned &box, const Sphere &sphere )
	{ // by Dan Andersson
		Float4 e = Max( box.minVertex - sphere.center, Float4::null );
		e += Max( sphere.center - box.maxVertex, Float4::null );

		if( e.Dot(e) > (sphere.radius * sphere.radius) ) return false;
		return true;
	}

	bool Intersect( const BoxAxisAligned &box, const Sphere &sphere, Float4 &worldPointOfContact )
	{ // by Robin Engman
		if( Intersect(box, sphere) )
		{
			Float distance;
			Float4 boxMiddle = (box.maxVertex - box.minVertex) * 0.5f;
			Ray ray( boxMiddle, (sphere.center - boxMiddle).Normalize() );
			Intersect( sphere, ray, distance );
			worldPointOfContact = ray.origin + ray.direction * distance;
			return true;
		}
	
		return false;
	}

	bool Intersect( const BoxAxisAligned &box, const Plane &plane )
	{ // by Dan Andersson
		Float e, d;
		Private::Compare( e, d, plane, box );
		if( d - e > 0.0f ) return false; // is beneath
		if( d + e < 0.0f ) return false; // is above
		return true;
	}

	bool Intersect( const BoxAxisAligned &box, const Plane &plane, Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement stub
		return Intersect( box, plane );
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
		Float4 dPos = point.center - box.center;

		Float coordinate = dPos.Dot( box.xAxis );
		if( coordinate > box.boundingOffset.x ) return false;
		if( coordinate < -box.boundingOffset.x ) return false;

		coordinate = dPos.Dot( box.yAxis );
		if( coordinate > box.boundingOffset.y ) return false;
		if( coordinate < -box.boundingOffset.y ) return false;

		coordinate = dPos.Dot( box.zAxis );
		if( coordinate > box.boundingOffset.z ) return false;
		if( coordinate < -box.boundingOffset.z ) return false;

		return true;
	}

	bool Intersect( const Box &box, const Point &point, Float4 &worldPointOfContact )
	{ // by Dan Andersson
		if( Intersect(box, point) )
		{
			worldPointOfContact = point.center;
			return true;
		}
		return false;
	}

	bool Intersect( const Box &box, const Ray &ray, Float &connectDistance )
	{ // by Dan Andersson
		Float tMin = ::std::numeric_limits<Float>::max(),
			  tMax = -tMin; // initiating to extremevalues

		Float4 dP = box.center - ray.origin;
		if( Private::BoxVsRayPerSlabCheck( box.xAxis, box.boundingOffset.x, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		if( Private::BoxVsRayPerSlabCheck( box.yAxis, box.boundingOffset.y, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		if( Private::BoxVsRayPerSlabCheck( box.zAxis, box.boundingOffset.z, dP, ray.direction, tMin, tMax ) ) { connectDistance = 0.0f; return false; }
		
		if( tMin > 0.0f ) connectDistance = tMin;
		else connectDistance = tMax;
		return true;
	}

	bool Intersect( const Box &box, const Ray &ray, Float &connectDistance, Float4 &worldPointOfContact )
	{ // by Dan Andersson
		if( Intersect(box, ray, connectDistance) )
		{
			worldPointOfContact = ray.origin + ray.direction * connectDistance;
			return true;
		}
		return false;
	}

	bool Intersect( const Box &box, const Sphere &sphere )
	{ // by Dan Andersson
		// center: sphere's center in the box's view space
		Float4 center = TransformVector( InverseRotationMatrix(box.rotation), sphere.center - box.center );

		Float4 e = Max( -box.boundingOffset - center, Float4::null );
		e += Max( center - box.boundingOffset, Float4::null );

		if( e.Dot(e) > (sphere.radius * sphere.radius) ) return false;
		return true;
	}

	bool Intersect( const Box &box, const Sphere &sphere, Float4 &worldPointOfContact )
	{ // by Robin Engman
		if( Intersect(box, sphere) )
		{
			Float distance;
			Ray ray( box.center, (sphere.center - box.center).Normalize() );
			
			Intersect( sphere, ray, distance );
			worldPointOfContact = ray.origin + ray.direction*distance;
			return true;
		}
		
		return false;
	}

	bool Intersect( const Box &box, const Plane &plane )
	{// by Dan Andersson
		Float e, d;
		Private::Compare( e, d, plane, box );
		if( d - e > 0.0f ) return false; // is beneath
		if( d + e < 0.0f ) return false; // is above
		return true;	
	}

	bool Intersect( const Box &box, const Plane &plane, Float4 &worldPointOfContact )
	{
		//! @todo TODO: implement stub
		return Intersect( box, plane );
	}

	bool Intersect( const Box &boxA, const BoxAxisAligned &boxB )
	{ // by Dan Andersson
		Float4 alignedOffsetBoundaries = (boxB.maxVertex - boxB.minVertex) * 0.5f,
			   offset = boxA.center- Average( boxB.maxVertex, boxB.minVertex );
		return Private::SeperatingAxisTest_AxisAlignedVsTransformedBox( alignedOffsetBoundaries, boxA.boundingOffset, boxA.rotation, offset );
	}

	bool Intersect( const Box &boxA, const BoxAxisAligned &boxB, ::Oyster::Math::Float4 &worldPointOfContact )
	{ // by Dan Andersson
		Float4 alignedOffsetBoundaries = (boxB.maxVertex - boxB.minVertex) * 0.5f,
			   offset = boxA.center - Average( boxB.maxVertex, boxB.minVertex );
		
		Float4 localPointOfContact;
		if( Private::SeperatingAxisTest_AxisAlignedVsTransformedBox( alignedOffsetBoundaries, boxA.boundingOffset, boxA.rotation, offset, localPointOfContact ) )
		{
			worldPointOfContact = localPointOfContact + boxA.center;
			worldPointOfContact.w = 1.0f;
			return true;
		}
		else return false;
	}

	bool Intersect( const Box &boxA, const Box &boxB )
	{ // by Dan Andersson
		Float4x4 rotationB = TransformMatrix( InverseRotationMatrix(boxA.rotation), boxB.rotation );
		Float4 posB = boxB.center - boxA.center;

		return Private::SeperatingAxisTest_AxisAlignedVsTransformedBox( boxA.boundingOffset, boxB.boundingOffset, rotationB, posB );
	}

	bool Intersect( const Box &boxA, const Box &boxB, Float4 &worldPointOfContact )
	{
		Float4x4 rotationB = TransformMatrix( InverseRotationMatrix(boxA.rotation), boxB.rotation );
		Float4 posB = boxB.center - boxA.center;

		Float4 localPointOfContact;
		if( Private::SeperatingAxisTest_AxisAlignedVsTransformedBox( boxA.boundingOffset, boxB.boundingOffset, rotationB, posB, localPointOfContact ) )
		{
			worldPointOfContact = TransformVector( boxA.rotation, localPointOfContact, localPointOfContact );
			worldPointOfContact += boxA.center;
			worldPointOfContact.w = 1.0f;
			return true;
		}
		else return false;
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
			connectDistance = Min( connectDistance, distance );
		}

		if( Intersect(frustrum.rightPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = Min( connectDistance, distance );
		}

		if( Intersect(frustrum.bottomPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = Min( connectDistance, distance );
		}

		if( Intersect(frustrum.topPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = Min( connectDistance, distance );
		}

		if( Intersect(frustrum.nearPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = Min( connectDistance, distance );
		}

		if( Intersect(frustrum.farPlane, ray, distance) )
		{
			intersected = true;
			connectDistance = Min( connectDistance, distance );
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

	Float TimeOfContact( const Sphere &protoStart, const Sphere &protoEnd, const Point &deuter )
	{ // Bisection with 5 levels of detail
		Float t = 0.5f,
			  d = 0.25f;
		Sphere s;
		for( int i = 0; i < 5; ++i )
		{
			Nlerp( protoStart, protoEnd, t, s );
			if( Intersect(s, deuter) )
			{
				t -= d;
			}
			else
			{
				t += d;
			}
			d *= 0.5f;
		}
		return t;
	}

	Float TimeOfContact( const Box &protoStart, const Box &protoEnd, const Point &deuter )
	{ // Bisection with 5 levels of detail
		Float t = 0.5f,
			  d = 0.25f;
		Box b;
		for( int i = 0; i < 5; ++i )
		{
			Nlerp( protoStart, protoEnd, t, b );
			if( Intersect(b, deuter) )
			{
				t -= d;
			}
			else
			{
				t += d;
			}
			d *= 0.5f;
		}
		return t;
	}

} } }