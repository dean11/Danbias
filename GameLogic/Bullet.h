/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#include "EffectCarrier.h"
#include "Line.h"

namespace GameLogic
{
	class Bullet : public EffectCarrier
	{
	public:
		class BulletCollector : public EffectCarrier::Collector
		{
		public:
			BulletCollector( const ::Oyster::Collision::Line *sampler, int capacity = 0 );
			virtual ~BulletCollector( );
			virtual void action( Object *hitObject );

			const ::Oyster::Collision::Line *sampler;
			::Oyster::Math::Float closest;
		};

		Bullet( Player *owner, const ::Oyster::Math::Float3 &origin, const ::Oyster::Math::Float3 &normalizedDirection, const ::Oyster::Math::Float &speed, Weapon::Type weapon, const ::Oyster::Math::Float &lifeTime = 0.0f );
		Bullet( const Bullet &bullet );
		virtual ~Bullet( );

		Bullet & operator = ( const Bullet &bullet );

		void setDirection( const ::Oyster::Math::Float3 &normalizedVector );
		void setSpeed( const ::Oyster::Math::Float &coordLengthPerSecond );
		void setLifeTime( const ::Oyster::Math::Float &seconds );
		void setModelID( int ID );

		Float3 getDirection() const;

		virtual State update( const ::Oyster::Math::Float &deltaTime );
		virtual const ::Oyster::Collision::ICollideable * getVolumeOfEffect( ) const;
		virtual EffectCarrier * clone( ) const;

		virtual Result writeToKeyFrame( KeyFrame &buffer ) const;
		virtual Result writeToNetEffect( Network::EffectData &buffer ) const;
		virtual Type getType( ) const { return EffectCarrier::Bullet; }

		virtual State onHit( const ::std::vector<Object*> &targets, const ::std::vector<::Oyster::Math::Float3> &hitPosW );

	private:
		::Oyster::Collision::Line lineOfEffect; // note current position is actually at the end of the lineOfEffect
		::Oyster::Math::Float speed, lifeCountDown;
		int modelID;
	};
}

#endif