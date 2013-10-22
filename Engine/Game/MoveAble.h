/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_MOVEABLE_H
#define GAME_MOVEABLE_H

#include "OysterMath.h"
#include "OysterCollision.h"

namespace Oyster { namespace Game
{
	class MoveAble
	{
	public:
		static void setDiscreteTimeSlice( const ::Oyster::Math::Float &seconds );
		static const ::Oyster::Math::Float & getDiscreteTimeSlice( );

		MoveAble( );
		MoveAble( const MoveAble &obj );
		MoveAble( const ::Oyster::Math::Float4x4 &orientation, const ::Oyster::Math::Float3 &rotationPivot = ::Oyster::Math::Float3::null );
		MoveAble( const ::Oyster::Collision::Box &body, const ::Oyster::Math::Float3 &rotationPivot = ::Oyster::Math::Float3::null );
		virtual ~MoveAble( );

		virtual MoveAble & operator = ( const MoveAble &obj );

		void accelerate( const ::Oyster::Math::Float3 &deltaSpeed, const ::Oyster::Math::Float3 &lever );
		void accelerate( const ::Oyster::Math::Float3 &deltaSpeed );
		void accelerateTurn( const ::Oyster::Math::Float3 &deltaRotationalSpeed );
		
		void stop( );
		void stopRotation( );
		void stopMovement( );

		void setSize( const ::Oyster::Math::Float3 &size );
		::Oyster::Math::Float3 getSize( ) const;

		virtual void setOrientation( const ::Oyster::Math::Float4x4 & orientation );
		const ::Oyster::Math::Float4x4 & getOrientation( ) const;
		
		::Oyster::Math::Float3 getMovement( const ::Oyster::Math::Float3 &offset ) const;
		const ::Oyster::Math::Float3 & getMovement( ) const;
		const ::Oyster::Math::Float3 & getRotation( ) const;

		virtual void update( );

	protected:
		::Oyster::Collision::Box body;
		::Oyster::Math::Float3 rotationPivot;

		::Oyster::Math::Float3 speed, rotationalSpeed;
		::Oyster::Math::Float3 acceleration, rotationalAcceleration;
	};
} }

#endif