/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_UNIVERSE_H
#define OYSTER_COLLISION_3D_UNIVERSE_H

#include "ICollideable.h"

namespace Oyster { namespace Collision3D
{
	class Universe : public ICollideable
	{
	public:
		Universe();
		virtual ~Universe();

		Universe & operator = ( const Universe &universe );

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Contains( const ICollideable &target ) const;
	};
} }

#endif