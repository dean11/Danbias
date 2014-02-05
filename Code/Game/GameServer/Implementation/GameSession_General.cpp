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
		this->logicDeltaTime = 0.0f;

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
		this->clients.Resize(desc.clients.Size());
		this->owner = desc.owner;

		/* Initiate the game instance */
		if(!this->gameInstance.Initiate())
		{
			printf("Failed to initiate the game instance\n");
		}

		/* Create the game level */
		if(!(this->levelData = this->gameInstance.CreateLevel()))
		{
			printf("Level not created!");
			return false;
		}

		/* Create the players in the game instance */
		GameLogic::IPlayerData* p = 0;
		for (unsigned int i = 0; i < desc.clients.Size(); i++)
		{
			if( (p = this->gameInstance.CreatePlayer()) )
			{
				desc.clients[i]->SetOwner(this);
				this->clients[i] = new GameClient(desc.clients[i], p);
			}
			else
			{
				printf("Failed to create player (%i)\n", i);
			}
		}

		/* Create the worker thread */
		if(this->worker.Create(this, false) != OYSTER_THREAD_ERROR_SUCCESS) 
			return false;

		this->worker.SetPriority(Oyster::Thread::OYSTER_THREAD_PRIORITY_3);

		/* Set some game instance data options */
		this->gameInstance.SetSubscription(GameLogic::GameEvent::ObjectEventFunctionType_OnMove, GameSession::ObjectMove);
		//this->gameInstance.SetSubscription(GameLogic::GameEvent::ObjectEventFunctionType_OnDead, GameSession::ObjectDead);

		this->description.clients.Clear();

		this->isCreated = true;
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
		//A timer so we dont lock because 1 client disconnected..
		Utility::WinTimer t;

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
				Protocol_LobbyStartGame p(readyList[i]->GetPlayer()->GetID(), "char_white.dan", readyList[i]->GetPlayer()->GetOrientation());
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
					//Need to send information about other players to all players
					for (unsigned int k = 0; k < readyList.Size(); k++)
					{
						if(k != i && this->clients[k])
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
	}

	bool GameSession::Attach(Utility::DynamicMemory::SmartPointer<NetworkClient> client)
	{
		if(!this->isCreated)	return false;

		client->SetOwner(this);
		SmartPointer<GameClient> obj = new GameClient(client, this->gameInstance.CreatePlayer());
	
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
		NetworkSession::CloseSession(true);
		this->clients.Clear();
	}


}//End namespace DanBias

