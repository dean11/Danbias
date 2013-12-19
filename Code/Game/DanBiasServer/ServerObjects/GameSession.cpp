
#include <GameProtocols.h>
#include <PostBox\PostBox.h>
#include "GameSession.h"
#include "ClientObject.h"
#include "..\Helpers\ProtocolParser.h"


#define ERIK

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;
using namespace Oyster;
using namespace Oyster::Thread;

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
	void GameSession::Send(::CustomNetProtocol& protocol) 
	{

	}
	void GameSession::Send(CustomNetProtocol& protocol, int ID) 
	{

	}
	void GameSession::SetPostbox(IPostBox<NetworkSession::NetEvent> *box) 
	{

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
	
////private:
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

#ifdef ERIK
	//VARIABLES GOES HERE
	
	
#endif

}//End namespace DanBias

