/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Point.h"
#include "..\OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Point::Point( ) : ICollideable(Type_point), center() {}
Point::Point( const Float3 &pos ) : ICollideable(Type_point), center(pos) {}
Point::~Point( ) {}

Point & Point::operator = ( const Point &point )
{
	this->center = point.center;
	return *this;
}

::Utility::Memory::UniquePointer<ICollideable> Point::Clone( ) const
{ return ::Utility::Memory::UniquePointer<ICollideable>( new Point(*this) ); }

bool Point::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_universe: return true;
	case Type_point: return Utility::Intersect( *this, *(Point*)target );
	case Type_ray: return Utility::Intersect( *(Ray*)target, *this, ((Ray*)target)->collisionDistance );
	case Type_sphere: Utility::Intersect( *(Sphere*)target, *this );
	case Type_plane: return Utility::Intersect( *(Plane*)target, *this );
	case Type_triangle: return false; // TODO: 
	case Type_box_axis_aligned: return Utility::Intersect( *(BoxAxisAligned*)target, *this );
	case Type_box: return Utility::Intersect( *(Box*)target, *this );
	case Type_frustrum: return false; // TODO: 
	default: return false;
	}
}

bool Point::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_point: return Utility::Intersect( *this, *(Point*)target );
	default: return false;
	}
}