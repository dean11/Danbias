/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Ray.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Ray::Ray( ) : ICollideable(Type_ray)
{
	this->origin = Float4::standard_unit_w;
	this->direction = Float4::standard_unit_z;
	this->collisionDistance = 0.0f;
}

Ray::Ray( const Float3 &o, const Float3 &d ) : ICollideable(Type_ray)
{
	this->origin = Float4( o, 1.0f );
	this->direction = Float4( d, 0.0f );
	this->collisionDistance = 0.0f;
}

Ray::Ray( const Float4 &o, const Float4 &d ) : ICollideable(Type_ray)
{
	this->origin = o;
	this->direction = d;
	this->collisionDistance = 0.0f;
}

Ray::~Ray( ) {}

Ray & Ray::operator = ( const Ray &ray )
{
	this->origin = ray.origin;
	this->direction = ray.direction;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Ray::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Ray(*this) );
}

bool Ray::Intersects( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_universe:
		this->collisionDistance = 0.0f;
		return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target, this->collisionDistance );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, this->collisionDistance, ((const Ray&)target).collisionDistance );
	case Type_sphere:			return Utility::Intersect( (const Sphere&)target, *this, this->collisionDistance );
	case Type_plane:			return Utility::Intersect( (const Plane&)target, *this, this->collisionDistance );
	//case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( (const BoxAxisAligned&)target, *this, this->collisionDistance );
	case Type_box:				return Utility::Intersect( (const Box&)target, *this, this->collisionDistance );
	//case Type_frustrum:			return false; // TODO: 
	default:					return false;
	}
}

bool Ray::Intersects( const ICollideable &target, Float4 &worldPointOfContact ) const
{
	switch( target.type )
	{
	case Type_universe:
		this->collisionDistance = 0.0f;
		worldPointOfContact = this->origin;
		return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target, this->collisionDistance, worldPointOfContact );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target,  this->collisionDistance, ((const Ray&)target).collisionDistance, worldPointOfContact );
	case Type_sphere:			return Utility::Intersect( (const Sphere&)target, *this, this->collisionDistance, worldPointOfContact );
	case Type_plane:			return Utility::Intersect( (const Plane&)target, *this, this->collisionDistance, worldPointOfContact );
	//case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( (const BoxAxisAligned&)target, *this, this->collisionDistance, worldPointOfContact );
	case Type_box:				return Utility::Intersect( (const Box&)target, *this, this->collisionDistance, worldPointOfContact );
	//case Type_frustrum:			return false; // TODO: 
	default:
		worldPointOfContact = Float3::null;
		return false;
	}
}

bool Ray::Contains( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_point:			return Utility::Intersect( *this, (const Point&)target, this->collisionDistance );
	case Type_ray:				return Utility::Contains( *this, (const Ray&)target );
	default:					return false;
	}
}

Float Ray::TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const
{
	if( deuterStart.type != deuterEnd.type )
		return -1.0f;

	switch( deuterStart.type )
	{ // TODO: more to implement
	case Type_universe:			return 0.0f;
	default:					return 1.0f;
	}
}