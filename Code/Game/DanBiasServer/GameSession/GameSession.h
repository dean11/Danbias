/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_GAME_SESSION_H
#define DANBIASSERVER_GAME_SESSION_H

//warning C4150: deletion of pointer to incomplete type, no destructor called, because of forward decleration and the use of smartpointer.
#pragma warning(disable: 4150)

#include "..\LobbySessions\NetworkSession.h"
#include <WinTimer.h>
#include <PostBox\IPostBox.h>
#include <Thread\OysterThread.h>
#include <GameAPI.h>
#include <Queue.h>

namespace DanBias
{
	class LobbyClient;
	class GameClient;
	class GameSession : public Oyster::Thread::IThreadObject, public INetworkSession
	{
	public:
		/**
		*	A container to use when initiating a new session
		*/
		struct GameDescription
		{
			std::wstring mapName;
			NetworkSession* owner;
			Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<LobbyClient>> clients;
		};
		static GameSession* gameSession; 
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
		bool Join(Utility::DynamicMemory::SmartPointer<LobbyClient> client);
		
		/**
		*	Closes the game session
		*	@param disconnectClients If set to true clients is dissconnected from the server, if false the clients is sent to the given owner of the session.
		*/
		void CloseSession(bool disconnectClients = false); 

		inline bool IsCreated() const	{ return this->isCreated; }
		inline bool IsRunning() const	{ return this->isRunning; }

		//Private member functions
	private:
		//Handles all events recieved
		void ParseEvents();
		//Handles all gameplay events
		void ParseGameplayEvent(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c);
		//Handles all general events
		void ParseGeneralEvent(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c);
		//Adds a client to the client list
		void InsertClient(Utility::DynamicMemory::SmartPointer<GameClient> obj);
		//Removes a client from the client list
		void RemoveClient(DanBias::GameClient* obj);
		//Sends a protocol ta all clients in session
		void Send(Oyster::Network::CustomNetProtocol* p);
		//Frame function, derived from IThreadObject
		bool DoWork	( ) override;
		//Sends a client to the owner, if obj is NULL then all clients is sent
		void SendToOwner(DanBias::GameClient* obj);
		//Do a cleanup on all the private data
		void Clean();
		//Update game objects if needed
		void UpdateGameObjects();

		//Private member variables
	private:
		
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<GameClient>> clients;
		Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box;
		Oyster::Thread::OysterThread worker;
		GameLogic::GameAPI& gameInstance;
		GameLogic::ILevelData *levelData;
		NetworkSession* owner;
		bool isCreated;
		bool isRunning;
		Utility::WinTimer timer;

		static void ObjectMove(GameLogic::IObjectData* movedObject);

	private:
		friend class AdminInterface;

	};//End GameSession
}//End namespace DanBias
#endif // !DANBIASSERVER_GAME_SESSION_H