/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_UTILITY_H
#define OYSTER_COLLISION_3D_UTILITY_H

#include "ICollideable.h"
#include "Universe.h"
#include "Point.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
//#include "Triangle.h"
#include "BoxAxisAligned.h"
#include "Box.h"
#include "Frustrum.h"
#include "Line.h"
#include "Cone.h"

namespace Oyster { namespace Collision3D { namespace Utility
{
	void Compare( ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float &connectOffsetSquared, const Ray &ray, const Point &point );
	void Compare( ::Oyster::Math::Float &connectDistanceA, ::Oyster::Math::Float &connectDistanceB, ::Oyster::Math::Float &connectOffsetSquared, const Ray &rayA, const Ray &rayB );
	void Compare( ::Oyster::Math::Float &connectOffset, const Plane &plane, const Point &point );

	bool Intersect( const Point &pointA, const Point &pointB );
	bool Intersect( const Point &pointA, const Point &pointB, ::Oyster::Math::Float4 &worldPointOfContact );

	bool Intersect( const Ray &ray, const Point &point, ::Oyster::Math::Float &connectDistance );
	bool Intersect( const Ray &ray, const Point &point, ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Ray &rayA, const Ray &rayB, ::Oyster::Math::Float &connectDistanceA, ::Oyster::Math::Float &connectDistanceB );
	bool Intersect( const Ray &rayA, const Ray &rayB, ::Oyster::Math::Float &connectDistanceA, ::Oyster::Math::Float &connectDistanceB, ::Oyster::Math::Float4 &worldPointOfContact );

	bool Intersect( const Sphere &sphere, const Point &point );
	bool Intersect( const Sphere &sphere, const Point &point, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Sphere &sphere, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool Intersect( const Sphere &sphere, const Ray &ray, ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Sphere &sphereA, const Sphere &sphereB );
	bool Intersect( const Sphere &sphereA, const Sphere &sphereB, ::Oyster::Math::Float4 &worldPointOfContact );

	bool Intersect( const Plane &plane, const Point &point );
	bool Intersect( const Plane &plane, const Point &point, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Plane &plane, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool Intersect( const Plane &plane, const Ray &ray, ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Plane &plane, const Sphere &sphere );
	bool Intersect( const Plane &plane, const Sphere &sphere, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Plane &planeA, const Plane &planeB );
	bool Intersect( const Plane &planeA, const Plane &planeB, ::Oyster::Math::Float4 &worldPointOfContact );

//	bool Intersect( const Triangle &triangle, const Point &point, ? );
//	bool Intersect( const Triangle &triangle, const Ray &ray, ? );
//	bool Intersect( const Triangle &triangle, const Sphere &sphere, ? );
//	bool Intersect( const Triangle &triangle, const Plane &plane, ? );
//	bool Intersect( const Triangle &triangleA, const Triangle &triangleB, ? );

	bool Intersect( const BoxAxisAligned &box, const Point &point );
	bool Intersect( const BoxAxisAligned &box, const Point &point, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const BoxAxisAligned &box, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool Intersect( const BoxAxisAligned &box, const Ray &ray, ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const BoxAxisAligned &box, const Sphere &sphere );
	bool Intersect( const BoxAxisAligned &box, const Sphere &sphere, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const BoxAxisAligned &box, const Plane &plane );
	bool Intersect( const BoxAxisAligned &box, const Plane &plane, ::Oyster::Math::Float4 &worldPointOfContact );
//	bool Intersect( const BoxAxisAligned &box, const Triangle &triangle );
	bool Intersect( const BoxAxisAligned &boxA, const BoxAxisAligned &boxB );

	bool Intersect( const Box &box, const Point &point );
	bool Intersect( const Box &box, const Point &point, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Box &box, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool Intersect( const Box &box, const Ray &ray, ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Box &box, const Sphere &sphere );
	bool Intersect( const Box &box, const Sphere &sphere, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Box &box, const Plane &plane );
	bool Intersect( const Box &box, const Plane &plane, ::Oyster::Math::Float4 &worldPointOfContact );
//	bool Intersect( const Box &box, const Triangle &triangle, ? );
	bool Intersect( const Box &boxA, const BoxAxisAligned &boxB );
	bool Intersect( const Box &boxA, const BoxAxisAligned &boxB, ::Oyster::Math::Float4 &worldPointOfContact );
	bool Intersect( const Box &boxA, const Box &boxB );
	bool Intersect( const Box &boxA, const Box &boxB, ::Oyster::Math::Float4 &worldPointOfContact );
	
	bool Intersect( const Frustrum &frustrum, const Point &point );
	bool Intersect( const Frustrum &frustrum, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool Intersect( const Frustrum &frustrum, const Sphere &sphere );
	bool Intersect( const Frustrum &frustrum, const Plane &plane );
//	bool Intersect( const Frustrum &frustrum, const Triangle &triangle, ? );
	bool Intersect( const Frustrum &frustrum, const BoxAxisAligned &box );
	bool Intersect( const Frustrum &frustrum, const Box &box );
	bool Intersect( const Frustrum &frustrumA, const Frustrum &frustrumB );

	bool Contains( const Ray &container, const Ray &ray );

	bool Contains( const Sphere &container, const Sphere &sphere );
//	bool Contains( const Sphere &container, const Triangle &triangle );
//	bool Contains( const Sphere &container, const BoxAxisAligned &box );
//	bool Contains( const Sphere &container, const Box &box );
//	bool Contains( const Sphere &container, const Frustrum &frustrum );

	bool Contains( const Plane &container, const Point &point );
	bool Contains( const Plane &container, const Ray &ray );
	bool Contains( const Plane &container, const Plane &plane );
//	bool Contains( const Plane &container, const Triangle &triangle );

//	bool Contains( const Triangle &container, const Point &point );
//	bool Contains( const Triangle &container, const Triangle &triangle );

//	bool Contains( const BoxAxisAligned &container, const Sphere &sphere );
//	bool Contains( const BoxAxisAligned &container, const Triangle &triangle );
//	bool Contains( const BoxAxisAligned &container, const BoxAxisAligned &box );
//	bool Contains( const BoxAxisAligned &container, const Box &box );
//	bool Contains( const BoxAxisAligned &container, const Frustrum &frustrum );

//	bool Contains( const Box &container, const Sphere &sphere );
//	bool Contains( const Box &container, const Triangle &triangle );
//	bool Contains( const Box &container, const BoxAxisAligned &box );
//	bool Contains( const Box &container, const Box &box );
//	bool Contains( const Box &container, const Frustrum &frustrum );

//	bool Contains( const Frustrum &container, const Sphere &sphere );
//	bool Contains( const Frustrum &container, const Triangle &triangle );
//	bool Contains( const Frustrum &container, const BoxAxisAligned &box );
//	bool Contains( const Frustrum &container, const Box &box );
//	bool Contains( const Frustrum &container, const Frustrum &frustrum );

	::Oyster::Math::Float TimeOfContact( const Sphere &protoStart, const Sphere &protoEnd, const Point &deuter );
	::Oyster::Math::Float TimeOfContact( const Box &protoStart, const Box &protoEnd, const Point &deuter );
} } }

#endif