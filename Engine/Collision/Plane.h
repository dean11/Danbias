/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_PLANE_H
#define OYSTER_COLLISION_PLANE_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision
{
	class Plane : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float3 normal; ::Oyster::Math::Float phasing; };
			::Oyster::Math::Float element[4];
			char byte[sizeof(::Oyster::Math::Float3) + sizeof(::Oyster::Math::Float)];
		};

		Plane( );
		Plane( const Plane &plane );
		Plane( const ::Oyster::Math::Float3 &normal, const ::Oyster::Math::Float &phasing );
		~Plane( );

		Plane & operator = ( const Plane &plane );

		ICollideable* clone( ) const;
		bool Intersects( const ICollideable *target ) const;
		bool Contains( const ICollideable *target ) const;
		ICollideable::State Advanced( const ICollideable *target ) const; //Not supported returns 0;
	};
} }

#endif