/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Ray.h"
#include "Collision.h"

using namespace ::Oyster::Collision;
using namespace ::Oyster::Math;

Ray::Ray( ) : ICollideable(ICollideable::Ray), origin(), direction(), collisionDistance(0.0f) {}
Ray::Ray( const Ray &ray ) : ICollideable(ICollideable::Ray), origin(ray.origin), direction(ray.direction), collisionDistance(0.0f) {}
Ray::Ray( const Float3 &o, const ::Oyster::Math::Float3 &d ) : ICollideable(ICollideable::Ray), origin(o), direction(d), collisionDistance(0.0f)  {}
Ray::~Ray( ) { /*Nothing needs to be done here*/ }

Ray & Ray::operator = ( const Ray &ray )
{
	this->origin = ray.origin;
	this->direction = ray.direction;
	this->collisionDistance = ray.collisionDistance;
	return *this;
}

ICollideable* Ray::clone( ) const
{ return new Ray( *this ); }

bool Ray::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target, this->collisionDistance );
	case ICollideable::Ray: return Utility::intersect( *this, *(Collision::Ray*)target, this->collisionDistance, ((Collision::Ray*)target)->collisionDistance );
	case ICollideable::Sphere: return Utility::intersect( *(Collision::Sphere*)target, *this, this->collisionDistance );
	case ICollideable::Plane: return Utility::intersect( *(Collision::Plane*)target, *this, this->collisionDistance );
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return Utility::intersect( *(Collision::BoxAxisAligned*)target, *this, this->collisionDistance );
	case ICollideable::Box: return Utility::intersect( *(Collision::Box*)target, *this, this->collisionDistance );
	case ICollideable::Frustrum: return false; // TODO
	default: return false;
	}
}

bool Ray::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target, this->collisionDistance );
	case ICollideable::Ray: Utility::contains( *this, *(Collision::Ray*)target );
	default: return false;
	}
}

ICollideable::State Ray::Advanced( const ICollideable *target ) const
{ return ICollideable::Missed; } //Not supported returns 0