/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "GameSession.h"
#include "GameClient.h"

#include <GameProtocols.h>
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
	void InsertObject(DynamicArray<SmartPointer<GameClient>>& clients, SmartPointer<GameClient> obj)
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
	void RemoveObject(DynamicArray<SmartPointer<GameClient>>& clients, DanBias::GameClient* obj)
	{
		for (unsigned int i = 0; i < clients.Size(); i++)
		{
			if(clients[i] && clients[i]->GetClient()->GetID() == obj->GetClient()->GetID())
			{
				clients[i] = 0;
				return;
			}
		}
	}

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
			this->clients[i] = new GameClient(desc.clients[i], c);
		}

		this->isCreated = true;
		return true;
	}

	void GameSession::Run()
	{
		if(this->isRunning) return;
		if(this->clients.Size() > 0)
		{
			if(this->worker.Create(this, true, true) != OYSTER_THREAD_ERROR_SUCCESS) return;
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

		SmartPointer<GameClient> obj = new GameClient(client, c);
		InsertObject(this->clients, obj);
			
		return true;
	}
	void GameSession::CloseSession(bool dissconnectClients)
	{
		this->worker.Stop(false);
		//NetworkSession::CloseSession(dissconnectClients);
		this->isCreated = false;
		this->isRunning = false;
	}

////private:	//overriden Threading functions
	bool GameSession::DoWork	( )
	{
		this->Frame();

		return true;
	}
	
#ifndef ERIK
////private:
	void GameSession::Frame()
	{
		this->ParseEvents();

		if(GetAsyncKeyState(VK_UP))
		{
			Protocol_General_Status p(Protocol_General_Status::States_ready);
			Send(p.GetProtocol());
			Sleep(100);
		}
	}
	void GameSession::ParseEvents()
	{
		if( !this->box->IsEmpty() )
		{
			NetworkSession::NetEvent &e = this->box->Fetch();
		
			if(e.protocol[0].type != Oyster::Network::NetAttributeType_Short) return;

			if( ProtocolIsGameplay(e.protocol[protocol_INDEX_ID].value.netShort) )
				ParseGameplayEvent(e.protocol, e.gameClient);

			if( ProtocolIsGeneral(e.protocol[protocol_INDEX_ID].value.netShort) )
				ParseGeneralEvent(e.protocol, e.gameClient);
		}
	}

	void GameSession::ParseGameplayEvent(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c)
	{
		switch (p[protocol_INDEX_ID].value.netShort)
		{
			case protocol_Gameplay_PlayerNavigation:
			{
				if(p[1].value.netBool)	//bool bForward;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_FORWARD);
				if(p[2].value.netBool)	//bool bBackward;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_BACKWARD);
				if(p[5].value.netBool)	//bool bStrafeRight;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_RIGHT);
				if(p[6].value.netBool)	//bool bStrafeLeft;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_LEFT);

				//Protocol_ObjectPosition op(c->GetPlayer()->GetOrientation(), c->GetPlayer()->GetID());
				//Send(op.GetProtocol());
			}
			break;
			case protocol_Gameplay_PlayerMouseMovement:
					
			break;
			case protocol_Gameplay_PlayerPosition:
					
			break;
			case protocol_Gameplay_CreateObject:
					
			break;
			case protocol_Gameplay_ObjectPosition:
					
			break;
		}
	}
	void GameSession::ParseGeneralEvent(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c)
	{
		switch (p[protocol_INDEX_ID].value.netShort)
		{
			case protocol_General_Status:
				switch (p[1].value.netInt)
				{
					case GameLogic::Protocol_General_Status::States_bussy:

					break;
					
					case GameLogic::Protocol_General_Status::States_disconected:
						printf("Client with ID [%i] dissconnected\n", c->GetClient()->GetID());
						RemoveObject(this->clients, c);
					break;

					case GameLogic::Protocol_General_Status::States_idle:
					break;

					case GameLogic::Protocol_General_Status::States_ready:

					break;
				}
			break;

			case protocol_General_Text:
					
			break;
		}
	}
	void GameSession::Send(Oyster::Network::CustomNetProtocol* p)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i] && this->clients[i]->GetClient())
				this->clients[i]->GetClient()->Send(p);
		}
	}

#else
#include "DynamicObject.h"
//#include "CollisionManager.h"
//#include "GameLogicStates.h"
//#include <GameProtocols.h>

	/*
	using namespace GameLogic;
	//VARIABLES GOES HERE
	DynamicObject* objectBox;

	bool GameSession::Init(GameSessionDescription& desc)
	{
		if(desc.clients.Size() == 0)	return false;
		this->box = new PostBox<NetEvent>();
		this->owner = desc.owner;
		for (unsigned int i = 0; i < desc.clients.Size(); i++)
		{
			desc.clients[i]->SetPostbox(this->box);
			this->clients.Push(desc.clients[i]);
		}

		CollisionManager::BoxCollision(0,0);

		objectBox = new DynamicObject(CollisionManager::BoxCollision, OBJECT_TYPE::OBJECT_TYPE_BOX);

		Protocol_CreateObject objectCreation;
		objectCreation.object_ID = objectBox->GetID();
		objectCreation.path = "crate";
		Oyster::Math::Float4x4 worldMat = objectBox->GetRigidBody()->GetOrientation();

		for (int i = 0; i < 16; i++)
		{
			objectCreation.worldMatrix[i] = worldMat[i];
		}

		for (int i = 0; i < clients.Size(); i++)
		{
			clients[i]->NetClient_Object()->Send(objectCreation);
		}

		return true;
	}
	void GameSession::Frame()
	{
	}
	void GameSession::ParseEvents()
	{
		if(this->box && !this->box->IsEmpty())
		{
			NetEvent &e = this->box->Fetch();

			if(e.protocol[0].type != Oyster::Network::NetAttributeType_Short) return;

			ParseProtocol(e.protocol, *e.reciever);
		}
	}
	void GameSession::ParseProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::ClientObject& c)
	{
		switch (p[0].value.netShort)
		{
		case protocol_Gamplay_PlayerNavigation:
			{
				if(p[1].value.netBool)	//bool bForward;
					c.Logic_Object()->Move(GameLogic::PLAYER_MOVEMENT_FORWARD);
				if(p[2].value.netBool)	//bool bBackward;
					c.Logic_Object()->Move(GameLogic::PLAYER_MOVEMENT_BACKWARD);
				if(p[5].value.netBool)	//bool bStrafeRight;
					c.Logic_Object()->Move(GameLogic::PLAYER_MOVEMENT_RIGHT);
				if(p[6].value.netBool)	//bool bStrafeLeft;
					c.Logic_Object()->Move(GameLogic::PLAYER_MOVEMENT_LEFT);
			}
			break;
		case protocol_Gamplay_PlayerMouseMovement:

			break;
		case protocol_Gamplay_PlayerPosition:

			break;
		case protocol_Gamplay_CreateObject:

			break;
		case protocol_Gamplay_ObjectPosition:

			break;
		}
	}
	*/
	
#endif

}//End namespace DanBias

