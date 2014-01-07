/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_GAME_SESSION_H
#define DANBIASSERVER_GAME_SESSION_H

//warning C4150: deletion of pointer to incomplete type, no destructor called
#pragma warning(disable: 4150)

#include "..\LobbySessions\NetworkSession.h"
#include <PostBox\IPostBox.h>
#include <Thread\OysterThread.h>

namespace DanBias
{
	class LobbyClient;
	class GameClient;
	class GameSession : public Oyster::Thread::IThreadObject, public INetworkSession
	{
	public:
		struct GameDescription
		{
			std::wstring mapName;
			NetworkSession* owner;
			Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<LobbyClient>> clients;
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
		bool Join(Utility::DynamicMemory::SmartPointer<LobbyClient> client);
		
		/**
		*	
		*/
		void CloseSession(bool dissconnectClients = false); 


		inline bool IsCreated() const	{ return this->isCreated; }
		inline bool IsRunning() const	{ return this->isRunning; }


	private:
		void Frame();
		void ParseEvents();
		void ParseGameplayEvent(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c);
		void ParseGeneralEvent(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c);

		void Send(Oyster::Network::CustomNetProtocol* p);

	private:	//overriden Threading functions
		bool DoWork	( ) override;

	private:
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<GameClient>> clients;
		NetworkSession* owner;
		Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box;
		Oyster::Thread::OysterThread worker;
		bool isCreated;
		bool isRunning;

	private:
		friend class AdminInterface;

	};//End GameSession
}//End namespace DanBias
#endif // !DANBIASSERVER_GAME_SESSION_H