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
			unsigned int maxClients;
			std::wstring mapName;
			std::wstring gameMode;
			int gameTimeMinutes;
			Oyster::Network::NetworkSession* owner;
			Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<GameClient>> clients;
		};

	public:
		GameSession();
		virtual~GameSession();

		/** Initiates and creates a game session. */
		bool Create(GameDescription& desc, bool forceStart);

		/** Runs the game session (ie starts the game loop). */
		void Run();

		/** Join an existing/running game session 
		*	@param client The client to attach to the session
		*/
		bool Join(gClient client);

		//void CloseSession( bool dissconnectClients ) override; 
		
		inline bool IsCreated() const	{ return this->isCreated; }
		inline bool IsRunning() const	{ return this->isRunning; }
		operator bool()					{ return (this->isCreated && this->isRunning); }

		//Private member functions
	private:
		// Client event callback function
		void ClientEventCallback(Oyster::Network::NetEvent<Oyster::Network::NetworkClient*, Oyster::Network::NetworkClient::ClientEventArgs> e) override;
		void ProcessClients() override;
		bool Send(Oyster::Network::CustomNetProtocol& message) override;
		bool Send(Oyster::Network::CustomNetProtocol& protocol, int ID) override;
		
		//Sends a client to the owner, if param is NULL then all clients is sent
		void SendToOwner(DanBias::GameClient* obj);
		
		//Derived from IThreadObject
		void ThreadEntry( ) override;
		bool DoWork	( ) override;


	private:
		void ParseProtocol					( Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c );

		void Gameplay_PlayerMovementRight	( DanBias::GameClient* c );
		void Gameplay_PlayerMovementLeft	( DanBias::GameClient* c );
		void Gameplay_PlayerMovementBack	( DanBias::GameClient* c );
		void Gameplay_PlayerMovementForth	( DanBias::GameClient* c );
		void Gameplay_PlayerJump			( DanBias::GameClient* c );
		void Gameplay_PlayerLeftTurn		( GameLogic::Protocol_PlayerLeftTurn& p, DanBias::GameClient* c );
		void Gameplay_PlayerChangeWeapon	( GameLogic::Protocol_PlayerChangeWeapon& p, DanBias::GameClient* c );
		void Gameplay_PlayerShot			( GameLogic::Protocol_PlayerShot& p, DanBias::GameClient* c );
		void Gameplay_ObjectPickup			( GameLogic::Protocol_ObjectPickup& p, DanBias::GameClient* c );
		void Gameplay_ObjectDamage			( GameLogic::Protocol_ObjectDamage& p, DanBias::GameClient* c );
		void Gameplay_ObjectPosition		( GameLogic::Protocol_ObjectPosition& p, DanBias::GameClient* c );
		void Gameplay_ObjectEnabled			( GameLogic::Protocol_ObjectEnable& p, DanBias::GameClient* c );
		void Gameplay_ObjectDisabled		( GameLogic::Protocol_ObjectDisable& p, DanBias::GameClient* c );
		void Gameplay_ObjectCreate			( GameLogic::Protocol_ObjectCreate& p, DanBias::GameClient* c );
		void General_Status					( GameLogic::Protocol_General_Status& p, DanBias::GameClient* c );
		void General_Text					( GameLogic::Protocol_General_Text& p, DanBias::GameClient* c );

		//Callback method recieving from gamelogic
		static void ObjectMove				( GameLogic::IObjectData* movedObject );
		static void ObjectDisabled			( GameLogic::IObjectData* movedObject );
		static void ObjectEnabled			( GameLogic::IObjectData* movedObject );
		static void ObjectDamaged			( GameLogic::IObjectData* movedObject, float hp );
		static void ObjectRespawned			( GameLogic::IObjectData* movedObject, Oyster::Math::Float3 spawnPos );
		static void ObjectDead				( GameLogic::IObjectData* movedObject, float seconds );
		static void PickupEvent				( GameLogic::IObjectData* movedObject, int pickupEffectID );
		static void ActionEvent				( GameLogic::IObjectData* movedObject , int actionID );
		//Private member variables
	private:
		Utility::DynamicMemory::DynamicArray<gClient> gClients;
		gClient sessionOwner;
		Oyster::Thread::OysterThread worker;
		GameLogic::GameAPI& gameInstance;
		GameLogic::ILevelData *levelData;
		NetworkSession* owner;
		bool isCreated;
		bool isRunning;
		float logicFrameTime;
		float networkFrameTime;
		Utility::WinTimer logicTimer;
		Utility::WinTimer networkTimer;
		GameDescription description;

		//TODO: Remove this uggly hax
		static GameSession* gameSession;


	};//End GameSession
}//End namespace DanBias
#endif // !DANBIASSERVER_GAME_SESSION_H