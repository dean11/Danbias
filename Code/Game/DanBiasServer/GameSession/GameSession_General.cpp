/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "GameSession.h"
#include "GameClient.h"
#include "..\GameServer.h"
#include <Protocols.h>
#include <PostBox\PostBox.h>
#include <GameLogicStates.h>

#include <Windows.h>


using namespace Utility::DynamicMemory;
using namespace Oyster;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace GameLogic;

namespace DanBias
{
	GameSession::GameSession()
	{
		this->owner = 0;
		this->box = 0;
		this->isCreated = false;
		this->isRunning = false;
	}

	GameSession::~GameSession()
	{
		delete this->box;
		this->box = 0;
		this->owner = 0;
	}

	bool GameSession::Create(GameDescription& desc)
	{
		if(desc.clients.Size() == 0)	return false;
		if(!desc.owner)					return false;
		if(!desc.mapName.size())		return false;
		if(this->isCreated)				return false;

		this->clients.Resize(desc.clients.Size());

		this->box = new PostBox<NetworkSession::NetEvent>();
		this->owner = desc.owner;

		Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> c;
		c.value.callbackPostBox = this->box;
		c.callbackType = Oyster::Callback::CallbackType_PostBox;

		for (unsigned int i = 0; i < desc.clients.Size(); i++)
		{
			this->clients[i] = new GameClient(desc.clients[i], gameInstance.CreatePlayer(), c);
		}

		if(this->worker.Create(this, true, true) != OYSTER_THREAD_ERROR_SUCCESS) return false;

		this->isCreated = true;

		return true;
	}

	void GameSession::Run()
	{

		if(this->isRunning) return;

		if(this->clients.Size() > 0)
		{
			
			this->worker.SetPriority(OYSTER_THREAD_PRIORITY_2);
			this->isRunning = true;
		}
	}

	bool GameSession::Join(Utility::DynamicMemory::SmartPointer<LobbyClient> client)
	{
		if(!this->isCreated)	return false;

		Oyster::Callback::OysterCallback<void, DanBias::NetworkSession::NetEvent> c;
		c.value.callbackPostBox = this->box;
		c.callbackType = Oyster::Callback::CallbackType_PostBox;

		SmartPointer<GameClient> obj = new GameClient(client, this->gameInstance.CreatePlayer(), c);
		AddClient(obj);
			
		return true;
	}

	void GameSession::CloseSession(bool dissconnectClients)
	{
		if(dissconnectClients)
		{
			for (int i = 0; i < this->clients.Size(); i++)
			{
				this->clients[i]->GetClient()->Disconnect();
			}
		}
		else
		{
			this->SendToOwner(0);	//Send all clients to the current owner
		}
		this->Clean();			
	}

	void GameSession::AddClient(SmartPointer<GameClient> obj)
	{
		for (unsigned int i = 0; i < clients.Size(); i++)
		{
			if(!clients[i])
			{
				clients[i] = obj;
				return;
			}
		}
		clients.Push(obj);
	}

	void GameSession::RemoveClient(DanBias::GameClient* obj)
	{
		for (unsigned int i = 0; i < clients.Size(); i++)
		{
			if(clients[i] && clients[i]->GetID() == obj->GetID())
			{
				clients[i] = 0;
				return;
			}
		}
	}

	void GameSession::SendToOwner(DanBias::GameClient* obj)
	{
		DanBias::NetworkSession *s = GameServer::MainLobbyInstance();
		
		if(this->owner)		s = this->owner;

		if(obj)
		{
			s->Attach(obj->ReleaseClient());
			RemoveClient(obj);
		}
		else
		{
			for (int i = 0; i < this->clients.Size(); i++)
			{
				if(this->clients[i])
				{
					s->Attach(this->clients[i]->ReleaseClient());
					RemoveClient(this->clients[i]);
				}
			}
		}
	}

	void GameSession::Clean()
	{
		this->worker.Terminate();
		this->clients.Clear();
		delete this->box;
		this->box = 0;
		this->gameInstance;
		this->owner = 0;
		this->isCreated = false;
		this->isRunning = false;
	}

}//End namespace DanBias

