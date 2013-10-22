/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_UTILITY_H
#define OYSTER_COLLISION_UTILITY_H

#include "ICollideable.h"
#include "Point.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
//#include "Triangle.h"
#include "BoxAxisAligned.h"
#include "Box.h"
#include "Frustrum.h"

namespace Oyster { namespace Collision { namespace Utility
{
	void compare( ::Oyster::Math::Float &connectDistance, ::Oyster::Math::Float &connectOffsetSquared, const Ray &ray, const Point &point );
	void compare( ::Oyster::Math::Float &connectDistanceA, ::Oyster::Math::Float &connectDistanceB, ::Oyster::Math::Float &connectOffsetSquared, const Ray &rayA, const Ray &rayB );
	void compare( ::Oyster::Math::Float &connectOffset, const Plane &plane, const Point &point );

	bool intersect( const Point &pointA, const Point &pointB );

	bool intersect( const Ray &ray, const Point &point, ::Oyster::Math::Float &connectDistance );
	bool intersect( const Ray &rayA, const Ray &rayB, ::Oyster::Math::Float &connectDistanceA, ::Oyster::Math::Float &connectDistanceB );

	bool intersect( const Sphere &sphere, const Point &point );
	bool intersect( const Sphere &sphere, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool intersect( const Sphere &sphereA, const Sphere &sphereB );

	bool intersect( const Plane &plane, const Point &point );
	bool intersect( const Plane &plane, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool intersect( const Plane &plane, const Sphere &sphere );
	bool intersect( const Plane &planeA, const Plane &planeB );

//	bool intersect( const Triangle &triangle, const Point &point, ? );
//	bool intersect( const Triangle &triangle, const Ray &ray, ? );
//	bool intersect( const Triangle &triangle, const Sphere &sphere, ? );
//	bool intersect( const Triangle &triangle, const Plane &plane, ? );
//	bool intersect( const Triangle &triangleA, const Triangle &triangleB, ? );

	bool intersect( const BoxAxisAligned &box, const Point &point );
	bool intersect( const BoxAxisAligned &box, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool intersect( const BoxAxisAligned &box, const Sphere &sphere );
	bool intersect( const BoxAxisAligned &box, const Plane &plane );
//	bool intersect( const BoxAxisAligned &box, const Triangle &triangle );
	bool intersect( const BoxAxisAligned &boxA, const BoxAxisAligned &boxB );

	bool intersect( const Box &box, const Point &point );
	bool intersect( const Box &box, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool intersect( const Box &box, const Sphere &sphere );
	bool intersect( const Box &box, const Plane &plane );
//	bool intersect( const Box &box, const Triangle &triangle, ? );
	bool intersect( const Box &boxA, const BoxAxisAligned &boxB );
	bool intersect( const Box &boxA, const Box &boxB );
	
	bool intersect( const Frustrum &frustrum, const Point &point );
	bool intersect( const Frustrum &frustrum, const Ray &ray, ::Oyster::Math::Float &connectDistance );
	bool intersect( const Frustrum &frustrum, const Sphere &sphere );
	bool intersect( const Frustrum &frustrum, const Plane &plane );
//	bool intersect( const Frustrum &frustrum, const Triangle &triangle, ? );
	bool intersect( const Frustrum &frustrum, const BoxAxisAligned &box );
	bool intersect( const Frustrum &frustrum, const Box &box );
	bool intersect( const Frustrum &frustrumA, const Frustrum &frustrumB );

	bool contains( const Ray &container, const Ray &ray );

	bool contains( const Sphere &container, const Sphere &sphere );
//	bool contains( const Sphere &container, const Triangle &triangle );
//	bool contains( const Sphere &container, const BoxAxisAligned &box );
//	bool contains( const Sphere &container, const Box &box );
//	bool contains( const Sphere &container, const Frustrum &frustrum );

	bool contains( const Plane &container, const Point &point );
	bool contains( const Plane &container, const Ray &ray );
	bool contains( const Plane &container, const Plane &plane );
//	bool contains( const Plane &container, const Triangle &triangle );

//	bool contains( const Triangle &container, const Point &point );
//	bool contains( const Triangle &container, const Triangle &triangle );

//	bool contains( const BoxAxisAligned &container, const Sphere &sphere );
//	bool contains( const BoxAxisAligned &container, const Triangle &triangle );
//	bool contains( const BoxAxisAligned &container, const BoxAxisAligned &box );
//	bool contains( const BoxAxisAligned &container, const Box &box );
//	bool contains( const BoxAxisAligned &container, const Frustrum &frustrum );

//	bool contains( const Box &container, const Sphere &sphere );
//	bool contains( const Box &container, const Triangle &triangle );
//	bool contains( const Box &container, const BoxAxisAligned &box );
//	bool contains( const Box &container, const Box &box );
//	bool contains( const Box &container, const Frustrum &frustrum );

//	bool contains( const Frustrum &container, const Sphere &sphere );
//	bool contains( const Frustrum &container, const Triangle &triangle );
//	bool contains( const Frustrum &container, const BoxAxisAligned &box );
//	bool contains( const Frustrum &container, const Box &box );
//	bool contains( const Frustrum &container, const Frustrum &frustrum );
} } }

#endif