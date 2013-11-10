/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Ray.h"
#include "OysterCollision.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Ray::Ray( ) : ICollideable(Type_ray), origin(), direction(), collisionDistance(0.0f) {}
Ray::Ray( const Float3 &o, const ::Oyster::Math::Float3 &d ) : ICollideable(Type_ray), origin(o), direction(d), collisionDistance(0.0f)  {}
Ray::~Ray( ) {}

Ray & Ray::operator = ( const Ray &ray )
{
	this->origin = ray.origin;
	this->direction = ray.direction;
	return *this;
}

::Utility::Memory::UniquePointer<ICollideable> Ray::Clone( ) const
{ return ::Utility::Memory::UniquePointer<ICollideable>( new Ray(*this) ); }

bool Ray::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_universe:
		this->collisionDistance = 0.0f;
		return true;
	case Type_point: return Utility::Intersect( *this, *(Point*)target, this->collisionDistance );
	case Type_ray: return Utility::Intersect( *this, *(Ray*)target, this->collisionDistance, ((Ray*)target)->collisionDistance );
	case Type_sphere: return Utility::Intersect( *(Sphere*)target, *this, this->collisionDistance );
	case Type_plane: return Utility::Intersect( *(Plane*)target, *this, this->collisionDistance );
	case Type_triangle: return false; // TODO: 
	case Type_box_axis_aligned: return Utility::Intersect( *(BoxAxisAligned*)target, *this, this->collisionDistance );
	case Type_box: return Utility::Intersect( *(Box*)target, *this, this->collisionDistance );
	case Type_frustrum: return false; // TODO: 
	default: return false;
	}
}

bool Ray::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_point: return Utility::Intersect( *this, *(Point*)target, this->collisionDistance );
	case Type_ray: Utility::Contains( *this, *(Ray*)target );
	default: return false;
	}
}