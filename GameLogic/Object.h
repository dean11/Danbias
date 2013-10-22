/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include "OysterMath.h"
#include "OysterCollision.h"
#include "OysterGame.h"
#include "Protocoll.h"
#include "InstanceBlueprint.h"

namespace GameLogic
{
	class Object : public ::Oyster::Game::MoveAble
	{
	public:
		enum Result { Success, Failure };
		enum Type { Player = 0, Static = 1, Destructable = 3, Undefined = 4 };
		static Result init( const ::std::string &iniFile ); // needed anymore?
		static Result importEntities( ::std::vector<unsigned int> &outputConfigID, const ::std::string &entityFile, const ::Oyster::Math::Float4x4 &transform = ::Oyster::Math::Float4x4::identity );
		static void clearEntityresources( );
		static unsigned int getNumObjectsCreated( );
		static const InstanceBlueprint &getConfig( unsigned int id );
		static const InstanceBlueprint &getConfig( const ::std::string &handle );

		static void onCollisionAction( Object *visitor, Object *hitEntity );

		static const unsigned int invalidID;

		struct
		{
			::Oyster::Math::Float maxSpeed, deAcceleration;
			struct{ ::Oyster::Math::Float forward, backward, horizontal, vertical; } acceleration;
		} movementProperty;

		struct
		{
			::Oyster::Math::Float maxSpeed, deAcceleration;
			struct{ ::Oyster::Math::Float pitch, yaw, roll; } acceleration;
		} rotationProperty;

		Object( );
		Object( const Object &object );
		Object( const ::Oyster::Math::Float4x4 &orientation, const ::Oyster::Math::Float3 &centerOfMass = ::Oyster::Math::Float3::null );
		Object( const ::Oyster::Collision::Box &body, const ::Oyster::Math::Float3 &centerOfMass = ::Oyster::Math::Float3::null );
		virtual ~Object( );

		Object & operator = ( const Object &object );
		virtual Object * clone( ) const;

		virtual Type getType( ) const;
		const unsigned int & getObjectID( ) const;
		const unsigned int & getConfigID( ) const;
		const ::Oyster::Game::CollisionHandler<Object>::Reference & getPhysicsID( ) const;
		const ::Oyster::Math::Float & getMass( ) const;
		const ::Oyster::Math::Float3 & getCenterOfMass( ) const;
		::Oyster::Math::Float3 getInertia( const ::Oyster::Math::Float3 &offset ) const;
		const ::Oyster::Collision::Box & getCollisionBox( ) const;
		bool isActive( ) const;

		void setConfig( unsigned int id );
		void setConfig( const ::std::string &id );
		void loadConfig( );

		void setPhysicsID( const ::Oyster::Game::CollisionHandler<Object>::Reference &id );
		void setMass( const ::Oyster::Math::Float &mass );
		void setCenterOfMass( const ::Oyster::Math::Float3 &localPos );
		void setScaling( const ::Oyster::Math::Float3 &scaleL );

		void applyForceL( const ::Oyster::Math::Float3 &localForce, const ::Oyster::Math::Float3 &localPos );
		void applyForceW( const ::Oyster::Math::Float3 &worldForce, const ::Oyster::Math::Float3 &worldPos );
		virtual void applyDamage( unsigned char magnitude, Object &attacker ) {}
		virtual void applyHeal( unsigned char magnitude ) {}
		virtual void HealShield( unsigned char magnitude ) {}
		virtual void DamageShield( unsigned char magnitude ) {}
		virtual void onDeath( Object &killer ) {}
		virtual void onKill( Object &victim ) {}

		void moveToLimbo( );
		void releaseFromLimbo( );

		virtual void update( );

		virtual void firePrimaryWeapon( ) {}
		virtual void fireSecondaryWeapon( ) {}
		virtual void fireSpecial( ) {}
		virtual void lockTarget( unsigned int objectID ) {}

		inline void activateMovementStabilisator( ) { this->enableMovementReduction( true ); }
		inline void deactivateMovementStabilisator( ) { this->disableMovementReduction(); }

		void pitchUp( );
		void pitchDown( );
		void yawLeft( );
		void yawRight( );
		void rollLeft( );
		void rollRight( );
		
		void thrustForward( );
		void thrustBackward( );
		void strafeLeft( );
		void strafeRight( );
		void climb( );
		void dive( );

		void disableMovementReduction( );
		void enableMovementReduction( bool forceIt = false );

		void disableRotationReduction( );
		void enableRotationReduction( bool forceIt = false );

		virtual Result writeToKeyFrame( KeyFrame &buffer ) const;


		const ::Oyster::Math::Float4x4 & getWorldPointMatrix( ) const;
		const ::Oyster::Math::Float4x4 & getViewPointMatrix( ) const;
		inline ::Oyster::Math::Float4x4 getWorldNormalMatrix( ) const { return this->getViewPointMatrix().getTranspose(); }
		inline ::Oyster::Math::Float4x4 getViewNormalMatrix( ) const { return this->getWorldPointMatrix().getTranspose(); }
		virtual void setOrientation( const ::Oyster::Math::Float4x4 & orientation );

	protected:
		static const InstanceBlueprint & getConfigData( unsigned int configID );

		virtual void loadConfig( const InstanceBlueprint &config );

	private:
		unsigned int objectID, configID;
		::Oyster::Game::CollisionHandler<Object>::Reference physicsID;
		::Oyster::Math::Float mass;
		int disableReduceMovementCount, disableReduceRotationCount;
		
		::Oyster::Math::Float3 scaling;
		mutable ::Oyster::Math::Float4x4 view, world; mutable bool viewIsOutOfDate, worldIsOutOfDate;
	};
}

namespace Oyster { namespace Game
{
	template <>
	bool CollisionHandler<::GameLogic::Object>::simpleCollisionConfirmation( const typename ::GameLogic::Object &entity, const ::Oyster::Collision::ICollideable *sampler );
} }

#endif