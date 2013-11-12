/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_SPHERE_H
#define OYSTER_COLLISION_3D_SPHERE_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision3D
{
	class Sphere : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float3 center; ::Oyster::Math::Float radius; };
			char byte[sizeof(::Oyster::Math::Float3) + sizeof(::Oyster::Math::Float)];
		};

		Sphere( );
		Sphere( const ::Oyster::Math::Float3 &center, const ::Oyster::Math::Float &radius );
		virtual ~Sphere( );

		Sphere & operator = ( const Sphere &sphere );

		virtual ::Utility::Memory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable *target ) const;
		bool Contains( const ICollideable *target ) const;
	};
} }

#endif