/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_POINT_H
#define OYSTER_COLLISION_POINT_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision
{
	class Point : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float3 position; };
			char byte[sizeof(::Oyster::Math::Float3)];
		};

		Point( );
		Point( const Point &point );
		Point( const ::Oyster::Math::Float3 &position );
		~Point( );

		Point & operator = ( const Point &point );

		ICollideable* clone( ) const;
		bool Intersects( const ICollideable *target ) const;
		bool Contains( const ICollideable *target ) const;
		ICollideable::State Advanced( const ICollideable *target ) const; //Not supported returns 0;
	};
} }

#endif