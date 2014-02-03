/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_FRUSTRUM_H
#define OYSTER_COLLISION_3D_FRUSTRUM_H

#include "OysterMath.h"
#include "ICollideable.h"
#include "Plane.h"

namespace Oyster { namespace Collision3D
{

	class Frustrum : public ICollideable
	{
	public:
		
		union
		{
			struct{ class Plane plane[6]; };
			struct
			{ class Plane leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane; };
		};

		Frustrum();
		Frustrum( const ::Oyster::Math::Float4x4 &viewProjection );
		virtual ~Frustrum();

		Frustrum & operator = ( const Frustrum &frustrum );
		Frustrum & operator = ( const ::Oyster::Math::Float4x4 &viewProjection );

		void Split( unsigned int numX, unsigned int numY, unsigned int numZ, Frustrum targetList[] ) const;
		void WriteToByte( unsigned int &nextIndex, unsigned char targetMem[] ) const;

		void Split( Frustrum targetList[], unsigned int numX, unsigned int numY = 1U, unsigned int numZ = 1u ) const; /// DEPRECATED
		void WriteToByte( unsigned char targetMem[], unsigned int &nextIndex ) const; /// DEPRECATED

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;
		bool Intersects( const ICollideable &target ) const;
		bool Intersects( const ICollideable &target, Oyster::Math::Float4 &worldPointOfContact ) const;
		bool Contains( const ICollideable &target ) const;

		::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const;

		::Oyster::Math::Float3 ExtractForwad();
	};

	// INLINE IMPLEMENTATIONS ///////////////////////////////////////

	inline void Frustrum::Split( Frustrum targetList[], unsigned int numX, unsigned int numY, unsigned int numZ ) const /// DEPRECATED
	{ this->Split( numX, numY, numZ, targetList ); }

	inline void Frustrum::WriteToByte( unsigned char targetMem[], unsigned int &nextIndex ) const /// DEPRECATED
	{ this->WriteToByte( nextIndex, targetMem ); }
} }

#endif