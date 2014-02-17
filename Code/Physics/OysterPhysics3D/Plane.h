/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_PLANE_H
#define OYSTER_COLLISION_3D_PLANE_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision3D
{
	class Plane : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float4 normal; ::Oyster::Math::Float phasing; };
			::Oyster::Math::Float element[5];
			char byte[sizeof(::Oyster::Math::Float4) + sizeof(::Oyster::Math::Float)];
		};

		Plane( );
		Plane( const ::Oyster::Math::Float4 &normal, const ::Oyster::Math::Float &phasing );
		virtual ~Plane( );

		Plane & operator = ( const Plane &plane );

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const;
		bool Contains( const ICollideable &target ) const;

		::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const;
	};
} }

#endif