/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/*
	Ray::direction is assumed to be normalized!
	- Dan
*/
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_RAY_H
#define OYSTER_COLLISION_3D_RAY_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision3D
{
	class Ray : public ICollideable
	{
	public:
		union
		{
			struct
			{
				::Oyster::Math::Float3 origin,
									   direction; /// Assumed to be normalized
			};
			char byte[2*sizeof(::Oyster::Math::Float3)];
		};
		mutable float collisionDistance;

		Ray( );
		Ray( const ::Oyster::Math::Float3 &origin, const ::Oyster::Math::Float3 &normalizedDirection );
		virtual ~Ray( );

		Ray & operator = ( const Ray &ray );

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Intersects( const ICollideable &target, const ::Oyster::Math::Float3 &worldPointOfContact ) const;
		bool Contains( const ICollideable &target ) const;
	};
} }

#endif