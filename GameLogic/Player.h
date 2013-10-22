/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAMELOGIC_PLAYER_H
#define GAMELOGIC_PLAYER_H

#include <string>
#include "Object.h"
#include "Weapon.h"

namespace GameLogic
{
	class Session;

	class Player : public Object
	{
		friend class Session;
	public:
		enum State { Vacant, Active, Dead, Limbo };
		static const unsigned char noTeam;

		Player( );
		Player( const Player &player );
		Player( const ::Oyster::Math::Float4x4 &orientation, const ::Oyster::Math::Float3 &centerOfMass = ::Oyster::Math::Float3::null );
		Player( const ::Oyster::Collision::Box &body, const ::Oyster::Math::Float3 &centerOfMass = ::Oyster::Math::Float3::null );
		virtual ~Player( );

		Player & operator = ( const Player &player );
		virtual Object * clone( ) const;

		virtual void update( float deltaTime );

		virtual void firePrimaryWeapon( );
		virtual void fireSecondaryWeapon( );
		virtual void fireSpecial( );
		virtual void lockTarget( unsigned int objectID );
		void setPrimaryWeapon(Weapon::Type weapon);
		void setPrimaryWeaponCooldown(float cooldown);

		virtual Object::Type getType( ) const;
		const ::std::string & getName( ) const;
		int getPlayerID( ) const;
		unsigned char getTeamID( ) const;
		unsigned int getNumKills( ) const;
		unsigned int getNumTeamKills( ) const;
		unsigned int getNumDeaths( ) const;
		Session *getSession( ) const;
		unsigned char getHullPoints( ) const;
		unsigned char getShieldPoints( ) const;

		void setName( const ::std::string &name );
		void setPlayerID( int id );
		void setTeam( unsigned char teamID );
		void setHullPoints( unsigned char value );
		void setShieldPoints( unsigned char value );
		void resetScores( );

		virtual void applyDamage( unsigned char magnitude, Object &attacker );
		virtual void applyHeal( unsigned char magnitude );
		virtual void onDeath( Object &killer );
		virtual void onKill( Object &victim );

		// teleports a player to a random unblocked spawnpoint and release from Limbo. Will end up in limbo if unable to find a spawnpoint
		void spawn( );

	protected:
		virtual void loadConfig( const InstanceBlueprint &config );

	private:
		Session *session;
		::std::string name;
		int playerID;
		unsigned int killCount, teamKillCount, deathCount;
		const Object *lockedTarget;
		unsigned char teamID, hullPoints, shieldPoints, specialID;
		Weapon::Type primaryWeapon;
		float primaryWeaponCooldown;
	};
}

#endif