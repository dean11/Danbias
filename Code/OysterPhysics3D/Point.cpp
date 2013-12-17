/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Point.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Point::Point( ) : ICollideable(Type_point)
{
	this->center = Float3::null;
}

Point::Point( const Float3 &pos ) : ICollideable(Type_point)
{
	this->center = pos;
}

Point::~Point( ) {}

Point & Point::operator = ( const Point &point )
{
	this->center = point.center;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Point::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Point(*this) );
}

bool Point::Intersects( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_universe:			return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	case Type_ray:				return Utility::Intersect( (const Ray&)target, *this, ((const Ray&)target).collisionDistance );
	case Type_sphere:			return Utility::Intersect( (const Sphere&)target, *this );
	case Type_plane:			return Utility::Intersect( (const Plane&)target, *this );
	//case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( (const BoxAxisAligned&)target, *this );
	case Type_box:				return Utility::Intersect( (const Box&)target, *this );
	//case Type_frustrum:			return false; // TODO: 
	default:					return false;
	}
}

bool Point::Intersects( const ICollideable &target, Float3 &worldPointOfContact ) const
{
	switch( target.type )
	{
	case Type_universe:
		worldPointOfContact = this->center;
		return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target, worldPointOfContact );
	case Type_ray:				return Utility::Intersect( (const Ray&)target, *this, ((const Ray&)target).collisionDistance, worldPointOfContact );
	case Type_sphere:			return Utility::Intersect( (const Sphere&)target, *this, worldPointOfContact );
	case Type_plane:			return Utility::Intersect( (const Plane&)target, *this, worldPointOfContact );
	//case Type_triangle:		return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( (const BoxAxisAligned&)target, *this, worldPointOfContact );
	case Type_box:				return Utility::Intersect( (const Box&)target, *this, worldPointOfContact );
	case Type_frustrum:			return false; // TODO: 
	default:
		worldPointOfContact = Float3::null;
		return false;
	}
}

bool Point::Contains( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	default:					return false;
	}
}