/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_SPHERE_H
#define OYSTER_COLLISION_3D_SPHERE_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster
{
	namespace Collision3D
	{
		class Sphere : public ICollideable
		{
		public:
			union
			{
				struct{ ::Oyster::Math::Float4 center; ::Oyster::Math::Float radius; };
				char byte[sizeof(::Oyster::Math::Float4) + sizeof(::Oyster::Math::Float)];
			};

			Sphere( );
			Sphere( const ::Oyster::Math::Float3 &center, const ::Oyster::Math::Float &radius );
			Sphere( const ::Oyster::Math::Float4 &center, const ::Oyster::Math::Float &radius );
			virtual ~Sphere( );

			Sphere & operator = ( const Sphere &sphere );

			virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
			bool Intersects( const ICollideable &target ) const;
			bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const;
			bool Contains( const ICollideable &target ) const;

			::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const;
		};
	}

	namespace Math
	{
		/********************************************************************
		 * Normalized Linear Interpolation
		 ********************************************************************/
		::Oyster::Collision3D::Sphere & Nlerp( const ::Oyster::Collision3D::Sphere &start, const ::Oyster::Collision3D::Sphere &end, ::Oyster::Math::Float t, ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() );
	}
}

#endif