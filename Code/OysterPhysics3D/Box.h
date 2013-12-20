/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_BOX_H
#define OYSTER_COLLISION_3D_BOX_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision3D
{
	class Box : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float4x4 rotation; ::Oyster::Math::Float4 center, boundingOffset; };
			struct
			{
				::Oyster::Math::Float4 xAxis;
				::Oyster::Math::Float4 yAxis;
				::Oyster::Math::Float4 zAxis;
			};
			char byte[sizeof(::Oyster::Math::Float4x4) + 2*sizeof(::Oyster::Math::Float4)];
		};

		Box( );
		Box( const ::Oyster::Math::Float4x4 &rotation, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &size );
		virtual ~Box( );

		Box & operator = ( const Box &box );

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const;
		bool Contains( const ICollideable &target ) const;
	};
} }

#endif