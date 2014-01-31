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


}//End namespace DanBias

