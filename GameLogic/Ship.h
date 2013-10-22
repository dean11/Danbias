/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_SHIP_H
#define GAME_SHIP_H

#include "Object.h"
#include "SoundSource.h"

namespace Game
{
	class Ship : public ::GameLogic::Object
	{
	public:
		Ship( const ::Oyster::Math::Float4x4 &orientation = ::Oyster::Math::Float4x4::identity, const ::Oyster::Math::Float3 &centerOfMass = ::Oyster::Math::Float3::null );
		virtual ~Ship( );

		virtual void update( const ::Oyster::Math::Float &deltaTime );
		const ::Oyster::Math::Float4x4 & getWorldMatrix( ) const;
		void setMutation( const ::Oyster::Math::Float4x4 &matrix );

		virtual void applyDamage( unsigned int magnitude, const Object &attacker );
		virtual void applyHeal( unsigned int magnitude );

	private:
		::Oyster::Math::Float4x4 transform;
		mutable ::Oyster::Math::Float4x4 world;
		mutable bool worldOutOfDate;
		::Oyster::Math::Float updatePeriod;

		SoundSource *engineSound;
	};
}

#endif