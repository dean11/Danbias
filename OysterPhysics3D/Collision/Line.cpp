/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Line.h"
#include "..\OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

Line::Line( ) : ICollideable(Type_line), ray(), length(0.0f) {}
Line::Line( const class Ray &_ray, const Float &_length ) : ICollideable(Type_line), ray(_ray), length(_length) {}
Line::Line( const Float3 &origin, const Float3 &normalizedDirection, const Float &_length ) : ICollideable(Type_line), ray(origin, normalizedDirection), length(_length) {}
Line::~Line( ) {}

Line & Line::operator = ( const Line &line )
{
	this->ray = line.ray;
	this->length = line.length;
	return *this;
}

::Utility::Memory::UniquePointer<ICollideable> Line::Clone( ) const
{ return ::Utility::Memory::UniquePointer<ICollideable>( new Line(*this) ); }

bool Line::Intersects( const ICollideable *target ) const
{
	if( target->type == Type_universe )
	{
		this->ray.collisionDistance = 0.0f;
		return true;
	}

	if( this->ray.Intersects( target ) ) if( this->ray.collisionDistance >= 0.0f ) if( this->ray.collisionDistance <= this->length )
		return true;
	
	this->ray.collisionDistance = 0.0f;
	return false;
}

bool Line::Contains( const ICollideable *target ) const
{ /* TODO: : */ return false; }
