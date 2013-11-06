/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_BOXAXISALIGNED_H
#define OYSTER_COLLISION_BOXAXISALIGNED_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision
{
	class BoxAxisAligned : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float3 minVertex, maxVertex; };
			char byte[2*sizeof(::Oyster::Math::Float3)];
		};

		BoxAxisAligned( );
		BoxAxisAligned( const BoxAxisAligned &box );
		BoxAxisAligned( const ::Oyster::Math::Float3 &minVertex, const ::Oyster::Math::Float3 &maxVertex );
		BoxAxisAligned( const ::Oyster::Math::Float &leftClip, const ::Oyster::Math::Float &rightClip, const ::Oyster::Math::Float &topClip, const ::Oyster::Math::Float &bottomClip, const ::Oyster::Math::Float &nearClip, const ::Oyster::Math::Float &farClip );
		~BoxAxisAligned( );

		BoxAxisAligned & operator = ( const BoxAxisAligned &box );

		ICollideable* clone( ) const;
		bool Intersects( const ICollideable *target ) const;
		bool Contains( const ICollideable *target ) const;
		ICollideable::State Advanced( const ICollideable *target ) const; //Not supported returns 0;
	};
} }

#endif