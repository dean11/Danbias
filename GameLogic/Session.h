/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include <set>
#include <map>
#include "OysterGame.h"
#include "OysterMath.h"
#include "Object.h"
#include "EffectCarrier.h"
#include "Player.h"
#include "Network.h"

namespace GameLogic
{
	class Session
	{
		friend class Player;
	public:
		enum InitResult { Success, Failure };
		enum State { MissingMap, Loading, Running, Updated, Idle, Over };
		
		Session( );
		~Session( );

		InitResult load( const ::std::string &mapFile );
		
		unsigned char getNumMaxPlayers( ) const;
		Player & accessPlayer( unsigned int id );
		void putInWorld( unsigned char playerID );
		void putInLimbo( unsigned char playerID );

		void addEffect( const EffectCarrier &effect );
		
		void writeToKeyFrame( KeyFrame &buffer );
		void writeToRenderResourceData( ::Protocol::RenderData &data );

		const State & update( const ::Oyster::Math::Float &deltaTime );

		void addEvent(Event::GameEvent *gameEvent);
		void fetchEvents(std::vector<Event::GameEvent*> &list);
		void fetchEffectData(std::vector<Network::EffectData> &effData);

		//No need for mutex etc, this should only be called before start
		void setNrPlayers(int pc){nrPlayers=pc;}
		void setKillsRequired(int kr){killsRequired=kr;}

		int CheckWinConditions();
	private:
		//Different from max nr, it's the actual count
		int nrPlayers;
		int killsRequired;
		::Oyster::Game::CollisionHandler<Object> world;
		::std::set<EffectCarrier*> effect;
		::std::map<unsigned int, Object*> objectIDLib;
		State state;
		Player player[8];
		::Oyster::Math::Float elapsedUpdatePeriod;

		::std::vector<::Oyster::Math::Float4x4> spawnPoint;
		::std::vector<Object> staticObjects;
		::std::string name, description, spawnPointOBJFile;

		std::vector<Event::GameEvent*> eventList;
	};
}

#endif