/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Plane.h"
#include "Collision.h"

using namespace ::Oyster::Collision;
using namespace ::Oyster::Math;

Plane::Plane( ) : ICollideable(ICollideable::Plane), normal(), phasing(0.0f) {}
Plane::Plane( const Plane &plane ) : ICollideable(ICollideable::Plane), normal(plane.normal), phasing(plane.phasing) {}
Plane::Plane( const Float3 &n, const Float &p ) : ICollideable(ICollideable::Plane), normal(n), phasing(p) {}
Plane::~Plane( ) { /*Nothing needs to be done here*/ }

Plane & Plane::operator = ( const Plane &plane )
{
	this->normal = plane.normal;
	this->phasing = plane.phasing;
	return *this;
}

ICollideable* Plane::clone( ) const
{ return new Plane( *this ); }

bool Plane::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Ray: return Utility::intersect( *this, *(Collision::Ray*)target, ((Collision::Ray*)target)->collisionDistance );
	case ICollideable::Sphere: return Utility::intersect( *this, *(Collision::Sphere*)target );
	case ICollideable::Plane: return Utility::intersect( *this, *(Collision::Plane*)target );
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return Utility::intersect( *(Collision::BoxAxisAligned*)target, *this );
	case ICollideable::Box: return Utility::intersect( *(Collision::Box*)target, *this );
	case ICollideable::Frustrum: return false; // TODO
	case ICollideable::Line: return false; // TODO
	default: return false;
	}
}

bool Plane::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Ray: return Utility::contains( *this, *(Collision::Ray*)target );
	case ICollideable::Plane: return Utility::contains( *this, *(Collision::Plane*)target );
	case ICollideable::Triangle: return false; // TODO
	default: return false;
	}
}

ICollideable::State Plane::Advanced( const ICollideable *target ) const
{ return ICollideable::Missed; } //Not supported returns 0