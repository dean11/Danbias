/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Box.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Box::Box( ) : ICollideable(Type_box), orientation(Float4x4::identity), boundingOffset() {}
Box::Box( const Float4x4 &o, const Float3 &s ) : ICollideable(Type_box), orientation(o), boundingOffset(s*0.5) {}
Box::~Box( ) {}

Box & Box::operator = ( const Box &box )
{
	this->orientation = box.orientation;
	this->boundingOffset = box.boundingOffset;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Box::Clone( ) const
{ return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Box(*this) ); }

bool Box::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_universe: return true;
	case Type_point: return Utility::Intersect( *this, *(Point*)target );
	case Type_ray: return Utility::Intersect( *this, *(Ray*)target, ((Ray*)target)->collisionDistance );
	case Type_sphere: return Utility::Intersect( *this, *(Sphere*)target );
	case Type_plane: return Utility::Intersect( *this, *(Plane*)target );
	// case Type_triangle: return false; // TODO: : 
	case Type_box_axis_aligned: return Utility::Intersect( *this, *(BoxAxisAligned*)target );
	case Type_box: return Utility::Intersect( *this, *(Box*)target );
	// case Type_frustrum: return false; // TODO: : 
	default: return false;
	}
}

bool Box::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_point: return Utility::Intersect( *this, *(Point*)target );
	// case Type_sphere: return false; // TODO: 
	// case Type_triangle: return false; // TODO: 
	// case Type_box_axis_aligned: return false; // TODO: 
	// case Type_box: return false; // TODO: 
	// case Type_frustrum: return false; // TODO: 
	default: return false;
	}
}