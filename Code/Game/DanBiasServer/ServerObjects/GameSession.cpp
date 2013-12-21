
#include <GameProtocols.h>
#include <PostBox\PostBox.h>
#include "GameSession.h"
#include "ClientObject.h"
#include <GameLogicStates.h>


using namespace Utility::DynamicMemory;
using namespace Oyster::Network;
using namespace Oyster;
using namespace Oyster::Thread;
using namespace GameLogic;

namespace DanBias
{
	GameSession::GameSession()
	{
		this->box = 0;
	}
	GameSession::~GameSession()
	{
		delete this->box;
		this->box = 0;
	}

	void GameSession::Run(GameSessionDescription& desc)
	{
		if(this->Init(desc))
		{
			if(this->worker.Create(this, true, true) != OYSTER_THREAD_ERROR_SUCCESS) return;
			this->worker.SetPriority(OYSTER_THREAD_PRIORITY_1);
		}
	}

	void GameSession::Join(Utility::DynamicMemory::SmartPointer<ClientObject> client)
	{
		AttachClient(client, this->box);
	}

////private:	overriden NetworkSession functions
	void GameSession::AttachClient(SmartPointer<ClientObject> client, Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box ) 
	{
		NetworkSession::AttachClient(client, box);
	}
	void GameSession::Close()
	{

	}
	SmartPointer<ClientObject> GameSession::DetachClient(NetworkClient* client) 
	{
		return SmartPointer<ClientObject>();
	}
	SmartPointer<ClientObject> GameSession::DetachClient(ClientObject* client) 
	{
		return SmartPointer<ClientObject>();
	}
	SmartPointer<ClientObject> GameSession::DetachClient(short ID) 
	{
		return SmartPointer<ClientObject>();
	}
	void GameSession::Send(CustomNetProtocol& protocol) 
	{
		NetworkSession::Send(protocol);
	}
	void GameSession::Send(CustomNetProtocol& protocol, int ID) 
	{
		NetworkSession::Send(protocol, ID);
	}
	void GameSession::SetPostbox(IPostBox<NetworkSession::NetEvent> *box) 
	{
		NetworkSession::SetPostbox(box);
	}
	void GameSession::CloseSession(NetworkSession* clientDestination) 
	{

	}

////private:	//overriden Threading functions
	void GameSession::ThreadEntry()
	{

	}
	void GameSession::ThreadExit()
	{

	}
	bool GameSession::DoWork	( )
	{
		this->ParseEvents();
		this->Frame();

		return true;
	}
	
#ifndef ERIK
////private:
	bool GameSession::Init(GameSessionDescription& desc)
	{
		if(desc.clients.Size() == 0)	return false;
		this->box = new PostBox<NetEvent>();
		this->owner = desc.owner;
		for (unsigned int i = 0; i < desc.clients.Size(); i++)
		{
			desc.clients[i]->SetPostbox(this->box);
			desc.clients[i]->CreatePlayer();
			this->clients.Push(desc.clients[i]);
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
		if( ProtocolIsGameplay(p[protocol_INDEX_ID].value.netShort) )
		{
			switch (p[protocol_INDEX_ID].value.netShort)
			{
				case protocol_Gameplay_PlayerNavigation:
				{

					if(p[1].value.netBool)	//bool bForward;
						c.GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_FORWARD);
					if(p[2].value.netBool)	//bool bBackward;
						c.GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_BACKWARD);
					if(p[5].value.netBool)	//bool bStrafeRight;
						c.GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_RIGHT);
					if(p[6].value.netBool)	//bool bStrafeLeft;
						c.GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_LEFT);

					//Oyster::Math::Float4x4 p;
					//Protocol_ObjectPosition op(c.GetPlayer()->GetRigidBody()->GetOrientation(p));
					//op.object_ID = c.GetPlayer()->GetID();
					//this->Send(*op.GetProtocol());
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
		else if(ProtocolIsGeneral(p[protocol_INDEX_ID].value.netShort) )
		{

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

