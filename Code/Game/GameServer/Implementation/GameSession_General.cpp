/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\GameSession.h"
#include "..\GameClient.h"
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
	this->owner = 0;
	this->isCreated = false;
	this->isRunning = false;
	this->gameSession = this;
	this->logicFrameTime = DELTA_TIME_20;
	this->networkFrameTime = DELTA_TIME_20;
	this->networkTimer.reset();
	this->logicTimer.reset();

	memset(&this->description, 0, sizeof(GameDescription));
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

bool GameSession::Create(GameDescription& desc)
{
	this->description = desc;
/* Do some error checking */
	if(desc.clients.Size() == 0)	return false;
	if(!desc.owner)					return false;
	if(this->isCreated)				return false;

/* standard initialization of some data */
	NetworkSession::clients = desc.clients;
	NetworkSession::clients.Resize((unsigned int)desc.maxClients);
	this->clients.Resize((unsigned int)desc.maxClients);
	this->owner = desc.owner;

/* Initiate the game instance */
	if(!this->gameInstance.Initiate())
	{
		printf("Failed to initiate the game instance\n");
	}

/* Create the players in the game instance */
	GameLogic::IPlayerData* p = 0;
	for (unsigned int i = 0; i < desc.clients.Size(); i++)
	{
		if(desc.clients[i])
		{
			if( (p = this->gameInstance.CreatePlayer()) )
			{
				desc.clients[i]->SetOwner(this);
				this->clients[i] = (new GameClient(desc.clients[i], p));
			}
			else
			{
				printf("Failed to create player (%i)\n", i);
			}
		}
	}

/* Create the game level */
	if(!(this->levelData = this->gameInstance.CreateLevel()))
	{
		printf("Level not created!");
		return false;
	}

/* Set some game instance data options */
	this->gameInstance.SetSubscription(GameSession::ObjectMove);
	this->gameInstance.SetSubscription(GameSession::ObjectDisabled);
	this->gameInstance.SetFPS(60);

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

	if(this->clients.Size() > 0)
	{
		this->worker.Start();
		this->worker.SetPriority(OYSTER_THREAD_PRIORITY_1);
		this->isRunning = true;
	}
}

void GameSession::ThreadEntry(  )
{
//List with clients that we are waiting on..
	DynamicArray<SmartPointer<GameClient>> readyList;// = this->clients;

//First we need to clean invalid clients, if any, and tell them to start loading game data
	for (unsigned int i = 0; i < this->clients.Size(); i++)
	{
		if(this->clients[i])
		{
			readyList.Push(this->clients[i]);
			Protocol_LobbyCreateGame p((char)1, (char)0, this->description.mapName);
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
				for (unsigned int k = 0; k < this->clients.Size(); k++)
				{
					if((this->clients[k] && readyList[i]) && readyList[i]->GetClient()->GetID() != this->clients[k]->GetClient()->GetID())
					{
						IPlayerData* pl = this->clients[k]->GetPlayer();
						Protocol_ObjectCreatePlayer p(	pl->GetPosition(), pl->GetRotation(), pl->GetScale(),
														pl->GetID(), true, this->clients[k]->GetPlayer()->GetTeamID(),
														/*nwClient->GetAlias()*/"", /*playerData->GetMesh()*/"char_white.dan");
						readyList[i]->GetClient()->Send(p);
					}
				}
					
				readyCounter-- ;
				readyList[i] = 0;
			}
		}
		Sleep(5); //TODO: This might not be needed here.
	}

//Sync with clients before starting countdown

	for (unsigned int i = 0; i < this->clients.Size(); i++)
	{
		if(this->clients[i])
		{
			this->clients[i]->GetClient()->Send(GameLogic::Protocol_LobbyStartGame(5));
		}
	}
}

bool GameSession::Attach(Utility::DynamicMemory::SmartPointer<NetworkClient> networkClient)
{
	if(!this->isCreated)									return false;
	if(this->GetClientCount() == this->clients.Capacity())	return false;

	networkClient->SetOwner(this);

	IPlayerData* playerData = this->gameInstance.CreatePlayer();
	if(!playerData) return false;

	SmartPointer<GameClient> gameClient = new GameClient(networkClient, playerData);
	NetworkClient* nwClient = gameClient->GetClient();
	
// Send the level information
	{
		Protocol_LobbyCreateGame lcg((char)1, (char)0, this->description.mapName);
		nwClient->Send(lcg);
	}

// Send the player data only
	{
		Protocol_ObjectCreatePlayer oc(	playerData->GetPosition(), playerData->GetRotation(), playerData->GetScale(), 
										playerData->GetID(), true, playerData->GetTeamID(), 
										/*nwClient->GetAlias()*/"", /*playerData->GetMesh()*/"char_white.dan");
		nwClient->Send(oc);
	}

// Send information about other clients
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(clients[i])
			{
				IPlayerData* temp = clients[i]->GetPlayer();
				Protocol_ObjectCreatePlayer oc(	temp->GetPosition(), temp->GetRotation(), temp->GetScale(), 
												temp->GetID(), false, temp->GetTeamID(), 
												/*nwClient->GetAlias()*/"", /*playerData->GetMesh()*/"char_white.dan");
				nwClient->Send(oc);
			}
		}
	}

//TODO: Need to be able to get the current gameplay data from the logic, to sync it with the client
	{

	}

// Insert the new client to the update list
	{
		bool added = false;
		for (unsigned int i = 0; !added && i < this->clients.Size(); i++)
		{
			if(!clients[i])
			{
				NetworkSession::clients[i] = networkClient;
				clients[i] = gameClient;
				added = true;
			}
		}
		if(!added)
		{
			NetworkSession::clients.Push( networkClient );
			clients.Push( gameClient );
		}
	}
// Send the start signal
	{
		nwClient->Send(GameLogic::Protocol_LobbyStartGame(0));
	}

	return true;
}

void GameSession::CloseSession( bool dissconnectClients )
{
	this->worker.Terminate();
	NetworkSession::CloseSession(true);
	this->clients.Clear();
}


