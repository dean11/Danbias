#include "NetworkIncludes.h"
#include "EventStructs.h"
#pragma once
#ifndef EVENT_H
#define EVENT_H
//There's a variable called eventList in Session.
//if you push_back any type of event, the server will pick them up in order after every update() is run.


//Try to keep events pointer-free. Trying to send a class with a pointer over a socket will result in a problem because
//it will send the pointer rather than the data in the pointer. That results in either 
//A: Program crashing because it's not allowed to read that space if it's on the same computer as the server or
//B: Program crashing because the pointer most probably points to an unused space if the client is on a separate computer.

//This is the basic event class.
//a position should always be given with all events, to use as a main location for sounds, effects, or whatever. We can remove it if we decide it's not needed later on.

namespace Event
{
	class GameEvent
	{
	protected:
	public:
		GameEvent(){}
		virtual int GetSize()=0;
		virtual void LoadRawData(char* d)=0;
		virtual void SaveRawData(char* d)=0;
	};

	//When a player fires a projectile, a BulletCreated event should be added.
	class BulletCreated : public GameEvent
	{
	private:
		EventStruct::BulletCreatedStruct data;
	public:
		BulletCreated():GameEvent(){}
		BulletCreated(int ownerID, Float3 position, Float3 Head);
		int GetOwner(){return data.owner;}
		int GetSize(){return sizeof(data);}
		EventStruct::BulletCreatedStruct GetAsStruct(){return data;}
		void LoadRawData(char* d);
		void SaveRawData(char* d);
	};

	class ScoreUpdate : public GameEvent
	{
	private:
		EventStruct::ScoreUpdateStruct data;
	public:
		ScoreUpdate():GameEvent(){}
		ScoreUpdate(Score* scoreboard);
		int GetSize(){return sizeof(data);}
		EventStruct::ScoreUpdateStruct GetAsStruct(){return data;}
		void LoadRawData(char* d);
		void SaveRawData(char* d);
	};

	//When some kind of player-fired projectile hits an enemy, a BulletHit even should be added.
	class BulletHit : public GameEvent
	{
	private:
		EventStruct::BulletHitStruct data;
	public:
		BulletHit():GameEvent(){}
		BulletHit(int attacker, int hitPlayer);
		int getAttackerID(){return data.attackingTarget;}
		int getHitTargetID(){return data.hitTarget;}
		int GetSize(){return sizeof(data);}
		EventStruct::BulletHitStruct GetAsStruct(){return data;}
		void LoadRawData(char* d);
		void SaveRawData(char* d);
	};

	//Shipspawned event, for when a ship respawns.
	//In ShipSpawned, all data that the client requires should be given.
	class ShipSpawned : public GameEvent
	{
	private:
		EventStruct::ShipSpawnedStruct data;
	public:
		ShipSpawned():GameEvent(){}
		ShipSpawned(Float3 position, int id);
		int GetSize(){return sizeof(data);}
		EventStruct::ShipSpawnedStruct GetAsStruct(){return data;}
		void LoadRawData(char* d);
		void SaveRawData(char* d);
	};

	class ShipDestroyed : public GameEvent
	{
	public:
		EventStruct::ShipDestroyedStruct data;
	public:
		ShipDestroyed():GameEvent(){}
		ShipDestroyed(int pid, int kid);
		void setScore(int i, Score score);
		void setScore(int i, int k, int d, int tk);
		void sortScore();
		int getDestroyedID() const {return data.playerID;}
		int getAttackerID() const {return data.killerID;}
		EventStruct::ShipDestroyedStruct GetAsStruct(){return data;}

		int GetSize(){return sizeof(data);}
		void LoadRawData(char* d);
		void SaveRawData(char* d);
	};

	
	class GameEnded : public GameEvent
	{
	private:
		EventStruct::GameEndedStruct data;
		//Have some variables which shows scores of player, who won, etc
		//you just need the ids. Don't send names etc.
	public:
		GameEnded();
		GameEnded(int winner);
		void setScore(int i, Score score);
		void setScore(int i, int k, int d, int tk);
		void sortScore();
		int GetSize(){return sizeof(data);}
		EventStruct::GameEndedStruct GetAsStruct(){return data;}
		void LoadRawData(char* d);
		void SaveRawData(char* d);
	};

	enum Type
	{
		UNSUPPORTED_TYPE,
		eBulletCreated,
		eBulletHit,
		eShipSpawned,
		eGameEnded,
		eShipDestroyed,
		eScoreUpdate
	};
	Event::Type getEventType(Event::GameEvent* evt);



}
#endif