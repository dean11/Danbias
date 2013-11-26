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
			struct{ ::Oyster::Math::Float4x4 rotation; ::Oyster::Math::Float3 center, boundingOffset; };
			struct
			{
				::Oyster::Math::Float3 xAxis; ::Oyster::Math::Float paddingA;
				::Oyster::Math::Float3 yAxis; ::Oyster::Math::Float paddingB;
				::Oyster::Math::Float3 zAxis; ::Oyster::Math::Float paddingC;
			};
			char byte[sizeof(::Oyster::Math::Float4x4) + 2*sizeof(::Oyster::Math::Float3)];
		};

		Box( );
		Box( const ::Oyster::Math::Float4x4 &rotation, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &size );
		virtual ~Box( );

		Box & operator = ( const Box &box );

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Contains( const ICollideable &target ) const;
	};
} }

#endif