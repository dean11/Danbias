/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Line.h"
#include "Collision.h"

using namespace ::Oyster::Collision;
using namespace ::Oyster::Math;

Line::Line( ) : ICollideable(ICollideable::Line), ray(), length(0.0f) {}
Line::Line( const Line &_line ) : ICollideable(ICollideable::Line), ray(_line.ray), length(_line.length)  {}
Line::Line( const class Ray &_ray, const Float &_length ) : ICollideable(ICollideable::Line), ray(_ray), length(_length) {}
Line::Line( const Float3 &origin, const Float3 &normalizedDirection, const Float &_length ) : ICollideable(ICollideable::Line), ray(origin, normalizedDirection), length(_length) {}
Line::~Line( ) { /*Nothing needs to be done here*/ }

Line & Line::operator = ( const Line &line )
{
	this->ray = line.ray;
	this->length = line.length;
	return *this;
}

ICollideable* Line::clone( ) const
{ return new Line(*this); }

bool Line::Intersects( const ICollideable *target ) const
{
	if( this->ray.Intersects( target ) ) if( this->ray.collisionDistance >= 0.0f ) if( this->ray.collisionDistance <= this->length )
			return true;
	
	this->ray.collisionDistance = 0.0f;
	return false;
}

bool Line::Contains( const ICollideable *target ) const
{ /*TODO*/ return false; }

ICollideable::State Line::Advanced( const ICollideable *target ) const
{ return ICollideable::Missed; } //Not supported returns 0