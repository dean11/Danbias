/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Box.h"
#include "Collision.h"

using namespace ::Oyster::Collision;
using namespace ::Oyster::Math;

Box::Box( ) : ICollideable(ICollideable::Box), orientation(Float4x4::identity), boundingOffset() {}

Box::Box( const Box &box ) : ICollideable(ICollideable::Box), orientation(box.orientation), boundingOffset(box.boundingOffset)
{
	this->orientation = box.orientation;
}

Box::Box( const Float4x4 &o, const Float3 &s ) : ICollideable(ICollideable::Box), orientation(o), boundingOffset(s*0.5)
{
	this->orientation = o;
}
Box::~Box( ) { /*Nothing needs to be done here*/ }

Box & Box::operator = ( const Box &box )
{
	this->orientation = box.orientation;
	this->boundingOffset = box.boundingOffset;
	return *this;
}

ICollideable* Box::clone( ) const
{ return new Box( *this ); }

bool Box::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Ray: return Utility::intersect( *this, *(Collision::Ray*)target, ((Collision::Ray*)target)->collisionDistance );
	case ICollideable::Sphere: return Utility::intersect( *this, *(Collision::Sphere*)target );
	case ICollideable::Plane: return Utility::intersect( *this, *(Collision::Plane*)target );
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return Utility::intersect( *this, *(Collision::BoxAxisAligned*)target );
	case ICollideable::Box: return Utility::intersect( *this, *(Collision::Box*)target );
	case ICollideable::Frustrum: return false; // TODO
	default: return false;
	}
}

bool Box::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Sphere: return false; // TODO
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return false; // TODO
	case ICollideable::Box: return false; // TODO
	case ICollideable::Frustrum: return false; // TODO
	default: return false;
	}
}

ICollideable::State Box::Advanced( const ICollideable *target ) const
{ return ICollideable::Missed; } //Not supported returns 0