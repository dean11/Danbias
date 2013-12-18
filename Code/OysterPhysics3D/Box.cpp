/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Box.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Box::Box( ) : ICollideable(Type_box)
{
	this->rotation = Float4x4::identity;
	this->center =0.0f;
	this->boundingOffset = Float3(0.5f);
}

Box::Box( const Float4x4 &r, const Float3 &p, const Float3 &s ) : ICollideable(Type_box)
{
	this->rotation = r;
	this->center = p;
	this->boundingOffset = Float3(s*0.5);
}

Box::~Box( ) {}

Box & Box::operator = ( const Box &box )
{
	this->rotation = box.rotation;
	this->center = box.center;
	this->boundingOffset = box.boundingOffset;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Box::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Box(*this) );
}

bool Box::Intersects( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_universe:			return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, ((const Ray&)target).collisionDistance );
	case Type_sphere:			return Utility::Intersect( *this, (const Sphere&)target );
	case Type_plane:			return Utility::Intersect( *this, (const Plane&)target );
	// case Type_triangle:			return false; // TODO: : 
	case Type_box_axis_aligned:	return Utility::Intersect( *this, (const BoxAxisAligned&)target );
	case Type_box:				return Utility::Intersect( *this, (const Box&)target );
	// case Type_frustrum:			return false; // TODO: : 
	default:					return false;
	}
}

bool Box::Intersects( const ICollideable &target, Float3 &worldPointOfContact ) const
{
	switch( target.type )
	{
	case Type_universe:
		worldPointOfContact = this->center;
		return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target, worldPointOfContact );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, ((const Ray&)target).collisionDistance, worldPointOfContact );
	case Type_sphere:			return Utility::Intersect( *this, (const Sphere&)target, worldPointOfContact );
	case Type_plane:			return Utility::Intersect( *this, (const Plane&)target, worldPointOfContact );
	// case Type_triangle:			return false; // TODO: : 
	case Type_box_axis_aligned:	return Utility::Intersect( *this, (const BoxAxisAligned&)target, worldPointOfContact );
	case Type_box:				return Utility::Intersect( *this, (const Box&)target, worldPointOfContact );
	// case Type_frustrum:			return false; // TODO: : 
	default:					return false;
	}
}

bool Box::Contains( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	//case Type_sphere:				return false; // TODO: 
	//case Type_triangle:			return false; // TODO: 
	//case Type_box_axis_aligned:	return false; // TODO: 
	//case Type_box:				return false; // TODO: 
	//case Type_frustrum:			return false; // TODO: 
	default: return false;
	}
}