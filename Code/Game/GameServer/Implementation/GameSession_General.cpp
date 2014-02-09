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

namespace DanBias
{
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
		this->clients.Reserve(desc.clients.Size());
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
			if( (p = this->gameInstance.CreatePlayer()) )
			{
				desc.clients[i]->SetOwner(this);
				this->clients.Push(new GameClient(desc.clients[i], p));
			}
			else
			{
				printf("Failed to create player (%i)\n", i);
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
		DynamicArray<SmartPointer<GameClient>> readyList = this->clients;

	//First we need to clean invalid clients, if any, and tell them to start loading game data
		for (unsigned int i = 0; i < readyList.Size(); i++)
		{
			if(!readyList[i]) 
			{
				readyList.Remove(i);
			}
			else
			{
				Protocol_LobbyCreateGame p(readyList[i]->GetPlayer()->GetID(), "char_white.dan", readyList[i]->GetPlayer()->GetOrientation());
				readyList[i]->GetClient()->Send(p);
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
							Protocol_ObjectCreate p(this->clients[k]->GetPlayer()->GetOrientation(), this->clients[k]->GetPlayer()->GetID(), "char_white.dan"); //The model name will be custom later..
							readyList[i]->GetClient()->Send(p);
						}
					}
					
					readyCounter-- ;
					readyList[i] = 0;
				}
			}
			Sleep(5); //TODO: This might not be needed here.
		}

		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i])
			{
				this->clients[i]->GetClient()->Send(GameLogic::Protocol_LobbyStartGame(5));
			}
		}
	}

	bool GameSession::Attach(Utility::DynamicMemory::SmartPointer<NetworkClient> client)
	{
		if(!this->isCreated)	return false;

		client->SetOwner(this);

		IPlayerData* player = this->gameInstance.CreatePlayer();
		if(!player) return false;

		SmartPointer<GameClient> obj = new GameClient(client, player);
	
		for (unsigned int i = 0; i < clients.Size(); i++)
		{
			if(!clients[i])
			{
				clients[i] = obj;
				return true;
			}
		}

		clients.Push(obj);
			
		return true;
	}

	void GameSession::CloseSession( bool dissconnectClients )
	{
		this->worker.Terminate();
		NetworkSession::CloseSession(true);
		this->clients.Clear();
	}


}//End namespace DanBias

