/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_LINE_H
#define OYSTER_COLLISION_LINE_H

#include "OysterMath.h"
#include "ICollideable.h"
#include "Ray.h"

namespace Oyster { namespace Collision
{
	class Line : public ICollideable
	{
	public:
		union
		{	
			struct { class Ray ray; ::Oyster::Math::Float length; };
			char byte[sizeof(class Ray) + sizeof(::Oyster::Math::Float)];
		};

		Line( );
		Line( const Line &line );
		Line( const class Ray &ray, const ::Oyster::Math::Float &length );
		Line( const ::Oyster::Math::Float3 &origin, const ::Oyster::Math::Float3 &normalizedDirection, const ::Oyster::Math::Float &length );
		~Line( );

		Line & operator = ( const Line &line );

		ICollideable* clone( ) const;
		bool Intersects( const ICollideable *target ) const;
		bool Contains( const ICollideable *target ) const;
		ICollideable::State Advanced( const ICollideable *target ) const; //Not supported returns 0;
	};
} }

#endif