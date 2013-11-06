/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_FRUSTRUM_H
#define OYSTER_COLLISION_FRUSTRUM_H

#include "OysterMath.h"
#include "ICollideable.h"
#include "Plane.h"

namespace Oyster { namespace Collision
{

	class Frustrum : public ICollideable
	{
	public:
		
		union
		{
			struct{ class Plane plane[6]; };
			struct
			{ class Plane leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane; };
		};

		Frustrum( );
		Frustrum( const Frustrum &frustrum );
		Frustrum( const ::Oyster::Math::Float4x4 &viewProjection );
		~Frustrum( );

		Frustrum & operator = ( const Frustrum &frustrum );
		Frustrum & operator = ( const ::Oyster::Math::Float4x4 &viewProjection );

		void split( Frustrum targetList[], unsigned int numX, unsigned int numY = 1U, unsigned int numZ = 1u ) const;
		void writeToByte( unsigned char targetMem[], unsigned int &nextIndex ) const;

		ICollideable* clone( ) const;
		bool Intersects( const ICollideable *target ) const;
		bool Contains( const ICollideable *target ) const;
		ICollideable::State Advanced( const ICollideable *target ) const; //Not supported returns 0;
	};
} }

#endif