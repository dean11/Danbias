/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_GAME_SESSION_H
#define DANBIASSERVER_GAME_SESSION_H

//warning C4150: deletion of pointer to incomplete type, no destructor called, because of forward decleration and the use of smartpointer.
#pragma warning(disable: 4150)


#include "GameClient.h"
#include <WinTimer.h>
#include <PostBox\IPostBox.h>
#include <Thread\OysterThread.h>
#include <GameAPI.h>
#include <Queue.h>
#include <NetworkSession.h>
#include <DynamicArray.h>


namespace DanBias
{
	class GameSession	: public Oyster::Network::NetworkSession
						, public Oyster::Thread::IThreadObject
	{
	public:
		
		/**
		*	A container to use when initiating a new session
		*/
		struct GameDescription
		{
			std::wstring mapName;
			Oyster::Network::NetworkSession* owner;
			Utility::DynamicMemory::DynamicArray<Oyster::Network::NetClient> clients;
		};

	public:
		GameSession();
		virtual~GameSession();

		/** Initiates and creates a game session. */
		bool Create(GameDescription& desc);

		/** Runs the game session (ie starts the game loop). */
		void Run();

		/** Join an existing/running game session 
		*	@param client The client to attach to the session
		*/
		bool Attach(Oyster::Network::NetClient client) override;
		
		inline bool IsCreated() const	{ return this->isCreated; }
		inline bool IsRunning() const	{ return this->isRunning; }

		//Private member functions
	private:
		//Handles all events recieved
		//void ParseEvents();
		
		void ClientEventCallback(Oyster::Network::NetEvent<Oyster::Network::NetworkClient*, Oyster::Network::NetworkClient::ClientEventArgs> e) override;
		void ParseProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c);
		
		//Sends a client to the owner, if obj is NULL then all clients is sent
		void SendToOwner(DanBias::GameClient* obj);
		
		//Frame function, derived from IThreadObject
		bool DoWork	( ) override;
		
		//Private member variables
	private:
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<GameClient>> clients;
		Oyster::Thread::OysterThread worker;
		GameLogic::GameAPI& gameInstance;
		GameLogic::ILevelData *levelData;
		NetworkSession* owner;
		bool isCreated;
		bool isRunning;
		Utility::WinTimer timer;

		static void ObjectMove(GameLogic::IObjectData* movedObject);


	};//End GameSession
}//End namespace DanBias
#endif // !DANBIASSERVER_GAME_SESSION_H