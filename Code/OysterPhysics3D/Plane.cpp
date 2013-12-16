/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Plane.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math;

Plane::Plane( ) : ICollideable(Type_plane)
{
	this->normal = Float3::standard_unit_z;
	this->phasing = 0.0f;
}

Plane::Plane( const Float3 &n, const Float &p ) : ICollideable(Type_plane)
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
{ return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Plane(*this) ); }

bool Plane::Intersects( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_universe: return true;
	case Type_point: return Utility::Intersect( *this, *(Point*)&target );
	case Type_ray: return Utility::Intersect( *this, *(Ray*)&target, ((Ray*)&target)->collisionDistance );
	case Type_sphere: return Utility::Intersect( *this, *(Sphere*)&target );
	case Type_plane: return Utility::Intersect( *this, *(Plane*)&target );
	// case Type_triangle: return false; // TODO: 
	case Type_box_axis_aligned: return Utility::Intersect( *(BoxAxisAligned*)&target, *this );
	case Type_box: return Utility::Intersect( *(Box*)&target, *this );
	case Type_frustrum: return false; // TODO: 
	case Type_line: return false; // TODO: 
	default: return false;
	}
}

bool Plane::Intersects( const ICollideable &target, ::Oyster::Math::Float3 &worldPointOfContact ) const
{
	switch( target.type )
	{
	case Type_universe: return true;
	case Type_point: return Utility::Intersect( *this, *(Point*)&target, worldPointOfContact );
	case Type_ray: return Utility::Intersect( *this, *(Ray*)&target, ((Ray*)&target)->collisionDistance, worldPointOfContact );
	case Type_sphere: return Utility::Intersect( *this, *(Sphere*)&target, worldPointOfContact );
	case Type_plane: return Utility::Intersect( *(Plane*)&target, *this, worldPointOfContact );
	// case Type_triangle: return false; // TODO: 
	case Type_box_axis_aligned: return false; // return Utility::Intersect( *(BoxAxisAligned*)&target, *this, worldPointOfContact );
	case Type_box: return false; // return Utility::Intersect( *(Box*)&target, *this, worldPointOfContact );
	case Type_frustrum: return false; // TODO: 
	default: worldPointOfContact = NULL;
		return false;
	}
}

bool Plane::Contains( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_point: return Utility::Intersect( *this, *(Point*)&target );
	case Type_ray: return Utility::Contains( *this, *(Ray*)&target );
	case Type_plane: return Utility::Contains( *this, *(Plane*)&target );
	// case Type_triangle: return false; // TODO: 
	default: return false;
	}
}