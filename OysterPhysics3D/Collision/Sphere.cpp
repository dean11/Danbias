#include "Sphere.h"
#include "Collision.h"

using namespace ::Oyster::Collision;
using namespace ::Oyster::Math;

Sphere::Sphere( ) : ICollideable(ICollideable::Sphere), center(), radius(0.0f) { }
Sphere::Sphere( const Sphere &sphere ) : ICollideable(ICollideable::Sphere), center(sphere.center), radius(sphere.radius) {}
Sphere::Sphere( const Float3 &_position, const Float &_radius ) : ICollideable(ICollideable::Sphere), center(_position), radius(_radius) {}
Sphere::~Sphere( ) { /*Nothing needs to be done here*/ }

Sphere & Sphere::operator = ( const Sphere &sphere )
{
	this->center = sphere.center;
	this->radius = sphere.radius;
	return *this;
}

ICollideable* Sphere::clone( ) const
{ return new Sphere( *this ); }

bool Sphere::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Ray: return Utility::intersect( *this, *(Collision::Ray*)target, ((Collision::Ray*)target)->collisionDistance );
	case ICollideable::Sphere: Utility::intersect( *this, *(Collision::Sphere*)target );
	case ICollideable::Plane: return Utility::intersect( *(Collision::Plane*)target, *this );
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return Utility::intersect( *(Collision::BoxAxisAligned*)target, *this );
	case ICollideable::Box: return Utility::intersect( *(Collision::Box*)target, *this );
	case ICollideable::Frustrum: return false; // TODO
	default: return false;
	}
}

bool Sphere::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point:  return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Sphere: return Utility::contains( *this, *(Collision::Sphere*)target );
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return false; // TODO
	case ICollideable::Box: return false; // TODO
	case ICollideable::Frustrum: return false; // TODO
	default: return false;
	}
}

ICollideable::State Sphere::Advanced( const ICollideable *target ) const
{ return ICollideable::Missed; } //Not supported returns 0