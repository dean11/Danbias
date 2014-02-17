/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_POINT_H
#define OYSTER_COLLISION_3D_POINT_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision3D
{
	class Point : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float4 center; };
			char byte[sizeof(::Oyster::Math::Float4)];
		};

		Point( );
		Point( const ::Oyster::Math::Float3 &position );
		Point( const ::Oyster::Math::Float4 &position );
		virtual ~Point( );

		Point & operator = ( const Point &point );

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const;
		bool Contains( const ICollideable &target ) const;

		::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const;
	};
} }

#endif