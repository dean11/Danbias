/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/*
	Ray::direction is assumed to be normalized!
	- Dan
*/
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_RAY_H
#define OYSTER_COLLISION_RAY_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision
{
	class Ray : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float3 origin, direction; };
			char byte[2*sizeof(::Oyster::Math::Float3)];
		};
		mutable float collisionDistance;

		Ray( );
		Ray( const Ray &ray );
		Ray( const ::Oyster::Math::Float3 &origin, const ::Oyster::Math::Float3 &normalizedDirection );
		~Ray( );

		Ray & operator = ( const Ray &ray );

		ICollideable* clone( ) const;
		bool Intersects( const ICollideable *target ) const;
		bool Contains( const ICollideable *target ) const;
		ICollideable::State Advanced( const ICollideable *target ) const; //Not supported returns 0;
	};
} }

#endif