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
#include <Protocols.h>


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
			int mapNumber;
			int maxClients;
			int gameMode;
			int gameTime;
			std::string gameName;
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
		// TODO: find out what this method does..
		void ClientEventCallback(Oyster::Network::NetEvent<Oyster::Network::NetworkClient*, Oyster::Network::NetworkClient::ClientEventArgs> e) override;

		
		//Sends a client to the owner, if obj is NULL then all clients is sent
		void SendToOwner(DanBias::GameClient* obj);
		
		//Frame function, derived from IThreadObject
		bool DoWork	( ) override;

	private:
		void ParseProtocol					(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c);

		void Gameplay_PlayerMovement		( GameLogic::Protocol_PlayerMovement& p, DanBias::GameClient* c );
		void Gameplay_PlayerLookDir			( GameLogic::Protocol_PlayerLook& p, DanBias::GameClient* c );
		void Gameplay_PlayerChangeWeapon	( GameLogic::Protocol_PlayerChangeWeapon& p, DanBias::GameClient* c );
		void Gameplay_PlayerShot			( GameLogic::Protocol_PlayerShot& p, DanBias::GameClient* c );
		void Gameplay_PlayerJump			( GameLogic::Protocol_PlayerJump& p, DanBias::GameClient* c );
		void Gameplay_ObjectPickup			( GameLogic::Protocol_ObjectPickup& p, DanBias::GameClient* c );
		void Gameplay_ObjectDamage			( GameLogic::Protocol_ObjectDamage& p, DanBias::GameClient* c );
		void Gameplay_ObjectPosition		( GameLogic::Protocol_ObjectPosition& p, DanBias::GameClient* c );
		void Gameplay_ObjectEnabled			( GameLogic::Protocol_ObjectEnable& p, DanBias::GameClient* c );
		void Gameplay_ObjectDisabled		( GameLogic::Protocol_ObjectDisable& p, DanBias::GameClient* c );
		void Gameplay_ObjectCreate			( GameLogic::Protocol_ObjectCreate& p, DanBias::GameClient* c );
		void General_Status					( GameLogic::Protocol_General_Status& p, DanBias::GameClient* c );
		void General_Text					( GameLogic::Protocol_General_Text& p, DanBias::GameClient* c );
		
		//Callback method recieving from gamelogic
		static void ObjectMove(GameLogic::IObjectData* movedObject);

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
		GameDescription description;

		//TODO: Remove this uggly hax
		static GameSession* gameSession;

	};//End GameSession
}//End namespace DanBias
#endif // !DANBIASSERVER_GAME_SESSION_H