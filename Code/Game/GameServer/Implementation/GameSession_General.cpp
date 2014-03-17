/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\GameSession.h"
#include "..\GameClient.h"
#include "..\GameLobby.h"
#include <Protocols.h>
#include <PostBox\PostBox.h>
#include <GameLogicStates.h>

#define NOMINMAX
#include <Windows.h>
#include <Queue.h>
#include <future>

#define DELTA_TIME_20	0.05f
#define DELTA_TIME_24	0.04166666666666666666666666666667f
#define DELTA_TIME_30	0.03333333333333333333333333333333f
#define DELTA_TIME_60	0.01666666666666666666666666666667f
#define DELTA_TIME_120	0.00833333333333333333333333333333f


using namespace Utility::DynamicMemory;
using namespace Oyster;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace GameLogic;
using namespace DanBias;

GameSession* GameSession::gameSession = nullptr;

GameSession::GameSession()
	:gameInstance(GameAPI::Instance())
{
	this->reset = false;
	this->owner = 0;
	this->isCreated = false;
	this->isRunning = false;
	this->gameSession = this;
	this->logicFrameTime = 1.0f/120.0f;
	this->networkFrameTime = 1.0f/70.0f;
	this->accumulatedNetworkTime = 0.0f;
	this->networkTimer.reset();
	this->logicTimer.reset();
	this->timerSendClock = 0.0f;
	this->timeLeft = 0.0f;

	this->gameInstance.SetFrameTimeLength(this->logicFrameTime);

	// HACK to avoid mem leaks 
	//memset(&this->description, 0, sizeof(GameDescription));
}

GameSession::~GameSession()
{
	this->worker.Terminate();
	this->clients.Clear();
	this->gameInstance;
	this->owner = 0;
	this->isCreated = false;
	this->isRunning = false;
}

bool GameSession::Create(GameDescription& desc, bool forceStart)
{
	this->description = desc;
/* Do some error checking */
	if(!forceStart && desc.clients.Size() == 0)	return false;
	if(!desc.owner)								return false;
	if(this->isCreated)							return false;

/* standard initialization of some data */
	this->gClients.Reserve(desc.maxClients);
	for (unsigned int i = 0; i < desc.clients.Size(); i++)
	{
		if(desc.clients[i])
		{
			this->clientCount++;
			desc.clients[i]->SetOwner(this);
			this->gClients.Push(desc.clients[i]);
		}
	}
	this->owner = desc.owner;

/* Initiate the game instance */
	if(!this->gameInstance.Initiate())
	{
		printf("Failed to initiate the game instance\n");
	}

/* Create the players in the game instance */
	GameLogic::IPlayerData* p = 0;
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if( (p = this->gameInstance.CreatePlayer()) )
		{
			this->gClients[i]->SetPlayer(p);
		}
		else
		{
			printf("Failed to create player (%i)\n", i);
		}
	}

/* Create the game level */
	if(!(this->levelData = this->gameInstance.CreateLevel(this->description.mapName.c_str())))
	{
		printf("Level not created!");
		return false;
	}
	levelData->InitGameMode(desc.gameTimeMinutes * 60.0f, 300);

/* Set some game instance data options */
	this->gameInstance.SetMoveSubscription(GameSession::ObjectMove);
	this->gameInstance.SetDisableSubscription(GameSession::ObjectDisabled);
	this->gameInstance.SetEnableSubscription(GameSession::ObjectEnabled);
	this->gameInstance.SetHpSubscription(GameSession::ObjectDamaged);
	this->gameInstance.SetRespawnSubscription(GameSession::ObjectRespawned);
	this->gameInstance.SetDeadSubscription(GameSession::ObjectDead);
	this->gameInstance.SetActionSubscription(GameSession::ActionEvent);
	this->gameInstance.SetPickupSubscription(GameSession::PickupEvent);
	this->gameInstance.SetCollisionSubscription(GameSession::CollisionEvent);
	this->gameInstance.SetWeaponEnergySubscription(GameSession::EnergyUpdate);
	this->gameInstance.SetGameOverSubscription(GameSession::GameOver);
	this->gameInstance.SetBeamEffectSubscription(GameSession::BeamEffect);
	this->gameInstance.SetOnGameTimeTick(GameSession::OnGameTimeTick, 1.0f);

	this->description.clients.Clear();

	this->isCreated = true;

	/* Create the worker thread */
	if(this->worker.Create(this, false) != OYSTER_THREAD_ERROR_SUCCESS) 
		return false;

	return this->isCreated;
}

void GameSession::Run()
{
	if(this->isRunning) return;

	this->worker.Start();
	this->worker.SetPriority(OYSTER_THREAD_PRIORITY_0);
	this->isRunning = true;
	
}

void GameSession::ThreadEntry(  )
{
	if(this->reset) 
	{
		printf("Server is restarting...\n");
		if(!this->ResetAndContinue())
		{
			for (unsigned int i = 0; i < this->gClients.Size(); i++)
			{
				if(this->gClients[i])
				{
					this->gClients[i]->GetClient()->Send(Protocol_General_Status(Protocol_General_Status::State_serverShutdown));
				}
			}
			printf("Server failed to restart...\n");
		}
		printf("Server succesfully restarted...\n");
		return;
	}
//List with clients that we are waiting on..
	DynamicArray<gClient> readyList;// = this->clients;

//First we need to clean invalid clients, if any, and tell them to start loading game data
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(this->gClients[i])
		{
			readyList.Push(this->gClients[i]);
			Protocol_LobbyCreateGame p((char)1, (char)0, Utility::String::WStringToString(this->description.mapName, std::string()));
			readyList[readyList.Size() - 1]->GetClient()->Send(p);
		}
	}
	
	unsigned int readyCounter = readyList.Size();

//Sync with clients
	while (readyCounter != 0)
	{
		this->ProcessClients();
		for (unsigned int i = 0; i < readyList.Size(); i++)
		{
			if(readyList[i] && readyList[i]->IsReady())
			{
				//Need to send information about other players, to all players
				for (unsigned int k = 0; k < this->gClients.Size(); k++)
				{
					if((this->gClients[k] && readyList[i]) && readyList[i]->GetClient()->GetID() != this->gClients[k]->GetClient()->GetID())
					{
						IPlayerData* pl = this->gClients[k]->GetPlayer();
						Protocol_ObjectCreatePlayer p(	pl->GetPosition(), pl->GetRotation(), pl->GetScale(),
														pl->GetID(), true, this->gClients[k]->GetPlayer()->GetTeamID(),
														Utility::String::WStringToString(this->gClients[k]->GetAlias(), std::string()), 
														Utility::String::WStringToString(this->gClients[k]->GetCharacter(), std::string()));
						readyList[i]->GetClient()->Send(p);
					}
				}
					
				readyCounter-- ;
				readyList[i] = 0;
			}
		}
	}

//Sync with clients before starting countdown
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(this->gClients[i])
		{
			this->gClients[i]->GetClient()->Send(GameLogic::Protocol_LobbyStartGame(5.0f));
		}
	}
}

Thread::IThreadObject::ThreadCode GameSession::ThreadExit(  )
{
	if(this->reset)
	{
		return Thread::IThreadObject::ThreadCode_ResetNormal;
	}
	printf("Game session is terminating..\n");
	return Thread::IThreadObject::ThreadCode_Exit;
}

bool GameSession::Join(gClient gameClient)
{
	if(!this->isCreated)									return false;
	if(this->gClients.Size() == this->gClients.Capacity())	return false;

	//Find better solution that handles clients whom conneccts when a session is restarting....
	if(this->reset)
	{
		//Server is resseting, wait..

	}

	gameClient->SetOwner(this);

	IPlayerData* playerData = this->gameInstance.CreatePlayer();
	if(!playerData) return false;

	gameClient->SetPlayer(playerData);
	NetworkClient* nwClient = gameClient->GetClient();

// Send the player data only
	{
		Protocol_ObjectCreatePlayer oc(	playerData->GetPosition(), playerData->GetRotation(), playerData->GetScale(), 
										playerData->GetID(), true, playerData->GetTeamID(), 
										Utility::String::WStringToString(gameClient->GetAlias(), std::string()), 
										Utility::String::WStringToString(gameClient->GetCharacter(), std::string()));
		nwClient->Send(oc);
	}

// Send information about other clients
	{
		for (unsigned int i = 0; i < this->gClients.Size(); i++)
		{
			if(this->gClients[i] && !this->gClients[i]->IsInvalid())
			{
				// other Player
				IPlayerData* temp = this->gClients[i]->GetPlayer();
				Protocol_ObjectCreatePlayer p1(	temp->GetPosition(), temp->GetRotation(), temp->GetScale(), 
												temp->GetID(), false, temp->GetTeamID(), 
												Utility::String::WStringToString(this->gClients[i]->GetAlias(), std::string()), 
												Utility::String::WStringToString(this->gClients[i]->GetCharacter(), std::string()));
				nwClient->Send(p1);

				// Send player score
				Protocol_PlayerScore oldPlayerScore(temp->GetID(), temp->GetKills(), temp->GetDeaths());
				nwClient->Send(oldPlayerScore);

				// Send players current animation ID
				if (temp->GetState() == PLAYER_STATE_JUMPING ||	temp->GetState() == PLAYER_STATE_WALKING || temp->GetState() == PLAYER_STATE_IDLE )
				{
					Protocol_ObjectAction oldPlayerAction(temp->GetID(), temp->GetState());
					nwClient->Send(oldPlayerAction);
				}
			
				// new player
				temp = playerData;
				Protocol_ObjectCreatePlayer p2(	temp->GetPosition(), temp->GetRotation(), temp->GetScale(), 
												temp->GetID(), false, temp->GetTeamID(), 
												Utility::String::WStringToString(gameClient->GetAlias(), std::string()), 
												Utility::String::WStringToString(gameClient->GetCharacter(), std::string()));
				this->gClients[i]->GetClient()->Send(p2);

				Protocol_PlayerScore newPlayerScore(temp->GetID(), temp->GetKills(), temp->GetDeaths());
				this->gClients[i]->GetClient()->Send(newPlayerScore);
			}
		}
	}


	{
		DynamicArray<IObjectData*> objects;
		this->levelData->GetAllDynamicObjects(objects);
		for (unsigned int i = 0; i < objects.Size(); i++)
		{
			//Protocol_ObjectPosition p(movedObject->GetPosition(), id);
			Protocol_ObjectPositionRotation p(objects[i]->GetPosition(), objects[i]->GetRotation(), objects[i]->GetID());
			nwClient->Send(p.GetProtocol());
			if( objects[i]->IsDisabled())
			{
				Protocol_ObjectDisable d(objects[i]->GetID());
				nwClient->Send(d.GetProtocol());
			}
		}
	}

// Insert the new client to the update list
	gameClient->GetClient()->Send(GameLogic::Protocol_LobbyStartGame(0));	
	gameClient->SetState(GameClient::ClientState_Ready);
	this->gClients.Push( gameClient );
	this->clientCount++;

	return true;
}

float GameSession::GetElapsedSeconds() const
{
	return this->timeLeft;
}

// Use lambda method to get callback (if you want)
bool GameSession::SyncClients(float maxSec, void(*fnc)(gClient client))
{
	bool syncing = true;
	int readyCount = 0;
	bool allSynced = true;

	Utility::WinTimer syncTime;
	syncTime.reset();

	DynamicArray<gClient> list;

	for (unsigned int i = 0; i < gClients.Size(); i++)
	{
		if(gClients[i]) list.Push(gClients[i]);
	}

	while ( syncing )
	{
		this->ProcessClients();

		bool allReady = true;
		for (unsigned int i = 0; i < list.Size(); i++)
		{
			if(list[i])
			{
				
				if(list[i]->IsReady())
				{
					if( fnc ) fnc( list.Pop(i) );
				}
				else
				{
					allReady = false;
				}
			}
		}
		if( syncTime.getElapsedSeconds() >= maxSec )
		{
			//Continue
			syncing = false;
			if(list.Size() > 0)
				allSynced = false;
		}
	}

	return allSynced;
}

// There is one extra sync in this method compared to ResetAndContinue, so they are pretty mush the same..
bool GameSession::ResetAndWait()
{

// Clean old stuff 
//-----------------------------------------------------------------------------------
	this->gameInstance.Release();
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(this->gClients[i]) 
		{
			this->gClients[i]->SetReadyState(false);
			this->gClients[i]->SetSinceLastResponse(0.0f);
			this->gClients[i]->ResetFailedProtocolCount();
			this->gClients[i]->ReleasePlayer();
		}
	}

// Recreate stuff 
//-----------------------------------------------------------------------------------
	/* Initiate the game instance */
	if(!this->gameInstance.Initiate())  printf("Failed to initiate the game instance\n");

	/* Create the game level */
	if(!(this->levelData = this->gameInstance.CreateLevel(this->description.mapName.c_str())))
	{
		printf("Level not created!");
		return false;
	}
	levelData->InitGameMode(30, 300);
	//levelData->InitGameMode(this->description.gameTimeMinutes * 60.0f, 300);

	/* Create the players in the game instance */
	GameLogic::IPlayerData* p = 0;
	this->clientCount = 0;
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(this->gClients[i])
		{
			if( (p = this->gameInstance.CreatePlayer()) )	this->gClients[i]->SetPlayer(p);
			else											printf("Failed to create player (%i)\n", i);
			
			this->clientCount ++;
		}
	}


// Wait for syncing to complete -----------------------------------------------------------------------------------

	//Phase 1
	//auto fnc = [](gClient c) 
	//{ 
	//	//c->GetClient()->Send(
	//};
	//
	//bool result = this->SyncClients(8.0f, fnc);

//  Misc
//-----------------------------------------------------------------------------------


/* Set some game instance data options */
	this->gameInstance.SetMoveSubscription(GameSession::ObjectMove);
	this->gameInstance.SetDisableSubscription(GameSession::ObjectDisabled);
	this->gameInstance.SetEnableSubscription(GameSession::ObjectEnabled);
	this->gameInstance.SetHpSubscription(GameSession::ObjectDamaged);
	this->gameInstance.SetRespawnSubscription(GameSession::ObjectRespawned);
	this->gameInstance.SetDeadSubscription(GameSession::ObjectDead);
	this->gameInstance.SetActionSubscription(GameSession::ActionEvent);
	this->gameInstance.SetPickupSubscription(GameSession::PickupEvent);
	this->gameInstance.SetCollisionSubscription(GameSession::CollisionEvent);
	this->gameInstance.SetWeaponEnergySubscription(GameSession::EnergyUpdate);
	this->gameInstance.SetGameOverSubscription(GameSession::GameOver);
	this->gameInstance.SetOnGameTimeTick(GameSession::OnGameTimeTick, 1.0f);

	this->description.clients.Clear();

	this->isCreated = true;

	return this->isCreated;
}

bool GameSession::ResetAndContinue()
{
	this->clientCount = 0;

	DynamicArray<gClient> list;
	list.Reserve(this->gClients.Size());

// Clean old stuff  -----------------------------------------------------------------------------------
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(this->gClients[i]) 
		{
			this->gClients[i]->SetReadyState(false);
			this->gClients[i]->SetSinceLastResponse(0.0f);
			this->gClients[i]->ResetFailedProtocolCount();
			this->gClients[i]->ReleasePlayer();
			list.Push(this->gClients[i]);

			this->gClients[i] = 0;
		}
	}
	
	this->gameInstance.Release();

// Recreate stuff -----------------------------------------------------------------------------------

	/* Set some game instance data options */
	this->gameInstance.SetMoveSubscription(GameSession::ObjectMove);
	this->gameInstance.SetDisableSubscription(GameSession::ObjectDisabled);
	this->gameInstance.SetEnableSubscription(GameSession::ObjectEnabled);
	this->gameInstance.SetHpSubscription(GameSession::ObjectDamaged);
	this->gameInstance.SetRespawnSubscription(GameSession::ObjectRespawned);
	this->gameInstance.SetDeadSubscription(GameSession::ObjectDead);
	this->gameInstance.SetActionSubscription(GameSession::ActionEvent);
	this->gameInstance.SetPickupSubscription(GameSession::PickupEvent);
	this->gameInstance.SetCollisionSubscription(GameSession::CollisionEvent);
	this->gameInstance.SetWeaponEnergySubscription(GameSession::EnergyUpdate);
	this->gameInstance.SetGameOverSubscription(GameSession::GameOver);
	this->gameInstance.SetOnGameTimeTick(GameSession::OnGameTimeTick, 1.0f);

	/* Initiate the game instance */
	if(!this->gameInstance.Initiate())  printf("Failed to initiate the game instance\n");

	/* Create the game level */
	if(!(this->levelData = this->gameInstance.CreateLevel(this->description.mapName.c_str())))
	{
		printf("Level not created!");
		return false;
	}
	levelData->InitGameMode(this->description.gameTimeMinutes * 60.0f, 300);

	for (unsigned int i = 0; i < list.Size(); i++)
	{
		((GameLobby*)this->owner)->Attach(list[i]);
	}

	this->isCreated = true;
	this->reset = false;
	this->isRunning = true;

	return this->isCreated;
}


//DynamicArray<gClient> GameSession::CloseSession( bool dissconnectClients )
//{
//	this->worker.Terminate();
//	//TODO: Send clients to lobby
//
//	//for (unsigned int i = 0; i < this->gClients.Size(); i++)
//	//{
//	//	if(this->gClients[i])
//	//	{
//	//		((GameLobby*)this->owner)-> this->gClients[i]
//	//	}
//	//}
//
//	this->gClients.Clear();
//}


