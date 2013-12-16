/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Ray.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Ray::Ray( ) : ICollideable(Type_ray)
{
	this->origin = Float3::null;
	this->direction = Float3::standard_unit_z;
	this->collisionDistance = 0.0f;
}

Ray::Ray( const Float3 &o, const ::Oyster::Math::Float3 &d ) : ICollideable(Type_ray)
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
	case Type_point: return Utility::Intersect( *this, *(Point*)&target, this->collisionDistance );
	case Type_ray: return Utility::Intersect( *this, *(Ray*)&target, this->collisionDistance, ((Ray*)&target)->collisionDistance );
	case Type_sphere: return Utility::Intersect( *(Sphere*)&target, *this, this->collisionDistance );
	case Type_plane: return Utility::Intersect( *(Plane*)&target, *this, this->collisionDistance );
	// case Type_triangle: return false; // TODO: 
	case Type_box_axis_aligned: return Utility::Intersect( *(BoxAxisAligned*)&target, *this, this->collisionDistance );
	case Type_box: return Utility::Intersect( *(Box*)&target, *this, this->collisionDistance );
	case Type_frustrum: return false; // TODO: 
	default: return false;
	}
}

bool Ray::Intersects( const ICollideable &target, const ::Oyster::Math::Float3 &worldPointOfContact ) const
{
	switch( target.type )
	{
	case Type_universe:
		this->collisionDistance = 0.0f;
		return true;
	case Type_point: return Utility::Intersect( *this, *(Point*)&target, worldPointOfContact );
	case Type_ray: return Utility::Intersect( *this, *(Ray*)&target, ((Ray*)&target)->collisionDistance, worldPointOfContact );
	case Type_sphere: return Utility::Intersect( *this, *(Sphere*)&target, worldPointOfContact );
	case Type_plane: return Utility::Intersect( *(Plane*)&target, *this, worldPointOfContact );
	// case Type_triangle: return false; // TODO: 
	case Type_box_axis_aligned: return Utility::Intersect( *(BoxAxisAligned*)&target, *this, worldPointOfContact );
	case Type_box: return Utility::Intersect( *(Box*)&target, *this, worldPointOfContact );
	case Type_frustrum: return false; // TODO: 
	default: worldPointOfContact = NULL;
		return false;
	}
}

bool Ray::Contains( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_point: return Utility::Intersect( *this, *(Point*)&target, this->collisionDistance );
	case Type_ray: return Utility::Contains( *this, *(Ray*)&target );
	default: return false;
	}
}