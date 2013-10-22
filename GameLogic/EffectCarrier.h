/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_EFFECT_H
#define GAME_EFFECT_H

#include <vector>
#include "OysterMath.h"
#include "OysterCollision.h"
#include "OysterGame.h"
#include "Object.h"
#include "Protocoll.h"
#include "Weapon.h"
#include "Player.h"
#include "NetworkUpdateStructs.h"
namespace GameLogic
{
	class EffectCarrier
	{
	public:
		enum State { Deployed, Armed, Dead };
		enum Result { Success, Failure };
		enum Type { Undefined, Bullet };

		class Collector : public ::Oyster::Game::CollisionHandler<Object>::VisitingInstance
		{
		public:
			Collector( int capacity = 0 );
			virtual ~Collector( );
			virtual void action( Object *hitObject );

			::std::vector<Object*> hitObjects;
			::std::vector<Float3> hitPosW;
		};

		// STATIC FACTORIES /////////////////////////////////////////

		static EffectCarrier * createBullet( Player *owner,
											 Weapon::Type weapon,
											 const ::Oyster::Math::Float3 &originW,
											 const ::Oyster::Math::Float3 &normalizedDirectionW,
											 const ::Oyster::Math::Float  &speed,
											 const ::Oyster::Math::Float  &lifeTime,
											 const ::std::string &modelRef );

		// INSTANCE /////////////////////////////////////////////////

		EffectCarrier( Object *owner, Weapon::Type weapon = Weapon::Undefined );
		EffectCarrier( const EffectCarrier &carrier );
		virtual ~EffectCarrier( );

		EffectCarrier & operator = ( const EffectCarrier & carrier );

		void setState( const State &state );
		void setWeaponType( Weapon::Type weapon );

		Object * accessOwner( ) const;

		virtual State update( const ::Oyster::Math::Float &deltaTime ) = 0;
		virtual State onHit( const ::std::vector<Object*> &targets, const ::std::vector<::Oyster::Math::Float3> &hitPosW );
		virtual const ::Oyster::Collision::ICollideable * getVolumeOfEffect( ) const = 0;
		virtual EffectCarrier * clone( ) const = 0;
		virtual Result writeToKeyFrame( KeyFrame &buffer ) const = 0;
		virtual Result writeToNetEffect(Network::EffectData &data) const = 0;

		virtual Type getType( ) const { return Undefined; }

	protected:
		Weapon::Type weapon;
		State state;
		Object *owner;
	};
}

#endif