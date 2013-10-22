/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_BOX_H
#define OYSTER_COLLISION_BOX_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision
{
	class Box : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float4x4 orientation; ::Oyster::Math::Float3 boundingOffset; };
			struct
			{
				::Oyster::Math::Float3 xAxis; ::Oyster::Math::Float paddingA;
				::Oyster::Math::Float3 yAxis; ::Oyster::Math::Float paddingB;
				::Oyster::Math::Float3 zAxis; ::Oyster::Math::Float paddingC;
				::Oyster::Math::Float3 center;
			};
			char byte[sizeof(::Oyster::Math::Float4x4) + sizeof(::Oyster::Math::Float3)];
		};

		Box( );
		Box( const Box &box );
		Box( const ::Oyster::Math::Float4x4 &orientation, const ::Oyster::Math::Float3 &size );
		~Box( );

		Box & operator = ( const Box &box );

		ICollideable* clone( ) const;
		bool Intersects( const ICollideable *target ) const;
		bool Contains( const ICollideable *target ) const;
		ICollideable::State Advanced( const ICollideable *target ) const; //Not supported returns 0;
	};
} }

#endif