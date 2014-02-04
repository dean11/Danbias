/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_LINE_H
#define OYSTER_COLLISION_3D_LINE_H

#include "OysterMath.h"
#include "ICollideable.h"
#include "Ray.h"

namespace Oyster { namespace Collision3D
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
		Line( const class Ray &ray, const ::Oyster::Math::Float &length );
		Line( const ::Oyster::Math::Float3 &origin, const ::Oyster::Math::Float3 &normalizedDirection, const ::Oyster::Math::Float &length );
		virtual ~Line( );

		Line & operator = ( const Line &line );

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const;
		bool Contains( const ICollideable &target ) const;

		::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const;
	};
} }

#endif