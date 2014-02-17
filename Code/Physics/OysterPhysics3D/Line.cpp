/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Line.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Line::Line( ) : ICollideable(Type_line)
{
	this->ray = Ray();
	this->length = 0.0f;
}

Line::Line( const class Ray &_ray, const Float &_length ) : ICollideable(Type_line)
{
	this->ray = _ray;
	this->length = _length;
}

Line::Line( const Float3 &origin, const Float3 &normalizedDirection, const Float &_length ) : ICollideable(Type_line)
{
	this->ray = Ray( origin, normalizedDirection );
	this->length = _length;
}

Line::~Line( ) {}

Line & Line::operator = ( const Line &line )
{
	this->ray = line.ray;
	this->length = line.length;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Line::Clone( ) const
{ return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Line(*this) ); }

bool Line::Intersects( const ICollideable &target ) const
{
	if( target.type == Type_universe )
	{
		this->ray.collisionDistance = 0.0f;
		return true;
	}

	if( this->ray.Intersects(target) ) if( this->ray.collisionDistance >= 0.0f ) if( this->ray.collisionDistance <= this->length )
	{
		return true;
	}
	
	this->ray.collisionDistance = 0.0f;
	return false;
}

bool Line::Intersects( const ICollideable &target, Float4 &worldPointOfContact ) const
{
	if( target.type == Type_universe )
	{
		this->ray.collisionDistance = 0.0f;
		worldPointOfContact = this->ray.origin;
		return true;
	}

	if( this->ray.Intersects(target) ) if( this->ray.collisionDistance >= 0.0f ) if( this->ray.collisionDistance <= this->length )
	{
		worldPointOfContact = this->ray.origin + this->ray.direction * this->ray.collisionDistance;
		return true;
	}
	
	this->ray.collisionDistance = 0.0f;
	return false;
}

bool Line::Contains( const ICollideable &target ) const
{ /* TODO: : */ return false; }

Float Line::TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const
{
	if( deuterStart.type != deuterEnd.type )
		return -1.0f;

	switch( deuterStart.type )
	{ // TODO: more to implement
	case Type_universe:			return 0.0f;
	default:					return 1.0f;
	}
}