/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_BOXAXISALIGNED_H
#define OYSTER_COLLISION_3D_BOXAXISALIGNED_H

#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster { namespace Collision3D
{
	class BoxAxisAligned : public ICollideable
	{
	public:
		union
		{
			struct{ ::Oyster::Math::Float4 minVertex, maxVertex; };
			char byte[2*sizeof(::Oyster::Math::Float4)];
		};

		BoxAxisAligned( );
		BoxAxisAligned( const ::Oyster::Math::Float3 &minVertex, const ::Oyster::Math::Float3 &maxVertex );
		BoxAxisAligned( const ::Oyster::Math::Float &leftClip, const ::Oyster::Math::Float &rightClip, const ::Oyster::Math::Float &topClip, const ::Oyster::Math::Float &bottomClip, const ::Oyster::Math::Float &nearClip, const ::Oyster::Math::Float &farClip );
		virtual ~BoxAxisAligned( );

		BoxAxisAligned & operator = ( const BoxAxisAligned &box );

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const;
		bool Contains( const ICollideable &target ) const;

		::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const;
	};
} }

#endif