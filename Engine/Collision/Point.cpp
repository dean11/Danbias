/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Point.h"
#include "Collision.h"

using namespace ::Oyster::Collision;
using namespace ::Oyster::Math;

Point::Point( ) : ICollideable(ICollideable::Point), position() {}
Point::Point( const Point &point ) : ICollideable(ICollideable::Point), position(point.position) {}
Point::Point( const Float3 &pos ) : ICollideable(ICollideable::Point), position(pos) {}
Point::~Point( ) { /*Nothing needs to be done here*/ }

Point & Point::operator = ( const Point &point )
{
	this->position = point.position;
	return *this;
}

ICollideable* Point::clone( ) const
{ return new Point( *this ); }

bool Point::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Ray: return Utility::intersect( *(Collision::Ray*)target, *this, ((Collision::Ray*)target)->collisionDistance );
	case ICollideable::Sphere: Utility::intersect( *(Collision::Sphere*)target, *this );
	case ICollideable::Plane: return Utility::intersect( *(Collision::Plane*)target, *this );
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return Utility::intersect( *(Collision::BoxAxisAligned*)target, *this );
	case ICollideable::Box: return Utility::intersect( *(Collision::Box*)target, *this );
	case ICollideable::Frustrum: return false; // TODO
	default: return false;
	}
}

bool Point::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	default: return false;
	}
}

ICollideable::State Point::Advanced( const ICollideable *target ) const
{ return ICollideable::Missed; } //Not supported returns 0