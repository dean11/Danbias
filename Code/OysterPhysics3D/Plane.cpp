/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Plane.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math;

Plane::Plane( ) : ICollideable(Type_plane)
{
	this->normal = Float4::standard_unit_z;
	this->phasing = 0.0f;
}

Plane::Plane( const Float4 &n, const Float &p ) : ICollideable(Type_plane)
{
	this->normal = n;
	this->phasing = p;
}

Plane::~Plane( ) {}

Plane & Plane::operator = ( const Plane &plane )
{
	this->normal = plane.normal;
	this->phasing = plane.phasing;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Plane::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Plane(*this) );
}

bool Plane::Intersects( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_universe:			return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, ((const Ray&)target).collisionDistance );
	case Type_sphere:			return Utility::Intersect( *this, (const Sphere&)target );
	case Type_plane:			return Utility::Intersect( *this, (const Plane&)target );
	//case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( (const BoxAxisAligned&)target, *this );
	case Type_box:				return Utility::Intersect( (const Box&)target, *this );
	//case Type_frustrum:			return false; // TODO: 
	//case Type_line:				return false; // TODO: 
	default:					return false;
	}
}

bool Plane::Intersects( const ICollideable &target, Float4 &worldPointOfContact ) const
{
	switch( target.type )
	{
	case Type_universe:
		worldPointOfContact = this->normal * this->phasing;
		return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target, worldPointOfContact );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, ((const Ray&)target).collisionDistance, worldPointOfContact );
	case Type_sphere:			return Utility::Intersect( *this, (const Sphere&)target, worldPointOfContact );
	case Type_plane:			return Utility::Intersect( (const Plane&)target, *this, worldPointOfContact );
	//case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( (const BoxAxisAligned&)target, *this, worldPointOfContact );
	case Type_box:				return Utility::Intersect( (const Box&)target, *this, worldPointOfContact );
	//case Type_frustrum:			return false; // TODO:
	default:
		worldPointOfContact = Float3::null;
		return false;
	}
}

bool Plane::Contains( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_point:		return Utility::Intersect( *this, (const Point&)target );
	case Type_ray:			return Utility::Contains( *this, (const Ray&)target );
	case Type_plane:		return Utility::Contains( *this, (const Plane&)target );
	//case Type_triangle:		return false; // TODO: 
	default:				return false;
	}
}

Float Plane::TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const
{
	if( deuterStart.type != deuterEnd.type )
		return -1.0f;

	switch( deuterStart.type )
	{ // TODO: more to implement
	case Type_universe:			return 0.0f;
	default:					return 1.0f;
	}
}