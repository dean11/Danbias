/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Plane.h"
#include "OysterCollision.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math;

Plane::Plane( ) : ICollideable(Type_plane), normal(), phasing(0.0f) {}
Plane::Plane( const Float3 &n, const Float &p ) : ICollideable(Type_plane), normal(n), phasing(p) {}
Plane::~Plane( ) {}

Plane & Plane::operator = ( const Plane &plane )
{
	this->normal = plane.normal;
	this->phasing = plane.phasing;
	return *this;
}

::Utility::Memory::UniquePointer<ICollideable> Plane::Clone( ) const
{ return ::Utility::Memory::UniquePointer<ICollideable>( new Plane(*this) ); }

bool Plane::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_universe: return true;
	case Type_point: return Utility::Intersect( *this, *(Point*)target );
	case Type_ray: return Utility::Intersect( *this, *(Ray*)target, ((Ray*)target)->collisionDistance );
	case Type_sphere: return Utility::Intersect( *this, *(Sphere*)target );
	case Type_plane: return Utility::Intersect( *this, *(Plane*)target );
	case Type_triangle: return false; // TODO: 
	case Type_box_axis_aligned: return Utility::Intersect( *(BoxAxisAligned*)target, *this );
	case Type_box: return Utility::Intersect( *(Box*)target, *this );
	case Type_frustrum: return false; // TODO: 
	case Type_line: return false; // TODO: 
	default: return false;
	}
}

bool Plane::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_point: return Utility::Intersect( *this, *(Point*)target );
	case Type_ray: return Utility::Contains( *this, *(Ray*)target );
	case Type_plane: return Utility::Contains( *this, *(Plane*)target );
	case Type_triangle: return false; // TODO: 
	default: return false;
	}
}