
#include <GameProtocols.h>
#include "GameSession.h"
#include <PostBox\PostBox.h>
#include "ClientObject.h"

#include "DynamicObject.h"
#include "CollisionManager.h"
#include "GameLogicStates.h"


#define ERIK

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;
using namespace Oyster;

namespace DanBias
{
	GameSession::GameSession()
	{

	}
	GameSession::~GameSession()
	{

	}

	void GameSession::Run(const GameSessionDescription& desc)
	{

	}

	void GameSession::AttachClient(SmartPointer<ClientObject> client, Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box ) 
	{
		this->Init();
	}

////private:	//overriden NetworkSession functions
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
	void GameSession::Init()
	{
#ifdef ERIK
		EricLogicInitFunc();
#else
		
#endif
	}
	void GameSession::Frame()
	{
#ifdef ERIK
		EricLogicFrameFunc();
#else
		
#endif
	}
	void GameSession::ParseEvents()
	{
		if(this->box && !this->box->IsEmpty())
		{
			NetEvent &e = this->box->Fetch();

#ifdef ERIK
			EricsLogicTestingProtocalRecieved(e.reciever, e.protocol);
#else
			if(e.protocol[0].type != Oyster::Network::NetAttributeType_Short) return;

			short f = e.protocol[0].value.netShort;

			switch (f)
			{
				default:
					
				break;
			}
#endif
		}
	}


#pragma region TESTING

using namespace GameLogic;

	void ConvertToMovement(ClientObject* reciever, CustomNetProtocol& inputToConvert);
	

	//VARIABLES GOES HERE
	int i = 0;
	DynamicObject* objectBox;

		void GameSession::EricLogicInitFunc()
		{
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

		}
		void GameSession::EricLogicFrameFunc()
		{
			
		}
		void GameSession::EricsLogicTestingProtocalRecieved(ClientObject* reciever, CustomNetProtocol& protocol)
		{
			bool moved = false;
			switch (protocol[protocol_ID_INDEX].value.netShort)
			{
			case protocol_Gameplay_PlayerNavigation:
				ConvertToMovement(reciever, protocol);
				moved = true;
				break;
			}

			if (moved)
			{
				
				Protocol_ObjectPosition playerMovement;

				Oyster::Math::Float4x4 worldMat = reciever->Logic_Object()->GetRigidBody()->GetOrientation();
				playerMovement.object_ID = reciever->Logic_Object()->GetID();
				for (int i = 0; i < 16; i++)
				{
					playerMovement.worldMatrix[i] = worldMat[i]; 
				}

				reciever->NetClient_Object()->Send(playerMovement);
			}


		}


		void ConvertToMovement(ClientObject* reciever,CustomNetProtocol& inputToConvert)
		{	
			if (inputToConvert[1].value.netBool == true)
			{
				reciever->Logic_Object()->Move(PLAYER_MOVEMENT::PLAYER_MOVEMENT_FORWARD);
			}
			if (inputToConvert[2].value.netBool == true)
			{
				reciever->Logic_Object()->Move(PLAYER_MOVEMENT::PLAYER_MOVEMENT_BACKWARD);
			}

		}
#pragma endregion

}//End namespace DanBias

