/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Point.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Point::Point( ) : ICollideable(Type_point)
{
	this->center = Float4::standard_unit_w;
}

Point::Point( const Float3 &pos ) : ICollideable(Type_point)
{
	this->center = Float4( pos, 1.0f );
}

Point::Point( const Float4 &pos ) : ICollideable(Type_point)
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

bool Point::Intersects( const ICollideable &target, Float4 &worldPointOfContact ) const
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

Float Point::TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const
{
	if( deuterStart.type != deuterEnd.type )
		return -1.0f;

	switch( deuterStart.type )
	{ // TODO: more to implement
	case Type_sphere:			return Utility::TimeOfContact( (const Sphere&)deuterStart, (const Sphere&)deuterEnd, *this );
	case Type_box:				return Utility::TimeOfContact( (const Box&)deuterStart, (const Box&)deuterEnd, *this );
	case Type_universe:			return 0.0f;
	default:					return 1.0f;
	}
}