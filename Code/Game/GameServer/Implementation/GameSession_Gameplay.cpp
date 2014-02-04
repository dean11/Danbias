/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\GameSession.h"
#include "..\GameClient.h"

#include <Protocols.h>
#include <PostBox\PostBox.h>
#include <GameLogicStates.h>
#include <OysterMath.h>
#define NOMINMAX
#include <Windows.h>

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
	bool GameSession::DoWork(  )
	{
		if(this->isRunning)
		{
			float dt = (float)this->logicTimer.getElapsedSeconds();
			this->logicDeltaTime += dt;
			this->logicTimer.reset();
			while( logicDeltaTime >= DELTA_TIME_120 )
			{
				this->ProcessClients();
				this->gameInstance.NewFrame();

				logicDeltaTime -= DELTA_TIME_120;
			}
		}

		return this->isRunning;
	}

	void GameSession::ClientEventCallback(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e)
	{
		int temp = -1;
		//Find the idiot
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i]->Equals(e.sender))
			{
				temp = i;
			}
		}

		if(temp == -1)		
		{
			this->Detach(e.sender)->Disconnect();
			return;
		}
		SmartPointer<GameClient> cl = this->clients[temp];

		switch (e.args.type)
		{
			case NetworkClient::ClientEventArgs::EventType_Disconnect:
			break;
			case NetworkClient::ClientEventArgs::EventType_ProtocolFailedToRecieve:
			break;
			case NetworkClient::ClientEventArgs::EventType_ProtocolFailedToSend:
				printf("\t(%i : %s) - EventType_ProtocolFailedToSend\n", e.sender->GetID(), e.sender->GetIpAddress().c_str());	
				this->Detach(e.sender);
			break;
			case NetworkClient::ClientEventArgs::EventType_ProtocolRecieved:
				printf("\t(%i : %s) - EventType_ProtocolRecieved\n", e.sender->GetID(), e.sender->GetIpAddress().c_str());	
				this->ParseProtocol(e.args.data.protocol, cl);
			break;
		}
	}

	void GameSession::ObjectMove(GameLogic::IObjectData* movedObject)
	{
		if(gameSession->networkTimer.getElapsedSeconds() >= DELTA_TIME_60)
		{
			gameSession->networkTimer.reset();

			GameLogic::IObjectData* obj = movedObject;

			if(dynamic_cast<IPlayerData*> (movedObject))
			{
				int id = obj->GetID();
				Oyster::Math::Float4x4 world = obj->GetOrientation();

				Protocol_ObjectPosition p(world, id);
				GameSession::gameSession->Send(p.GetProtocol());
			}
			else if(dynamic_cast<GameLogic::ILevelData*>(obj))
			{
				obj = ((GameLogic::ILevelData*)movedObject)->GetObjectAt(0);
				if(obj)
				{
					if(obj->GetObjectType() == OBJECT_TYPE_WORLD)
					{
						int id = obj->GetID();
						Oyster::Math::Float4x4 world =obj->GetOrientation();
					
						Protocol_ObjectPosition p(world, id);
						gameSession->Send(p.GetProtocol());
					}
				}

				obj =((GameLogic::ILevelData*)movedObject)->GetObjectAt(1);
				if(obj)
				{
					if(obj->GetObjectType() == OBJECT_TYPE_BOX)
					{
						int id = obj->GetID();
						Oyster::Math::Float4x4 world = obj->GetOrientation();
						Protocol_ObjectPosition p(world, id);
						gameSession->Send(p.GetProtocol());
					}
				}
			
				obj =((GameLogic::ILevelData*)movedObject)->GetObjectAt(2);
				if(obj)
				{
					if(obj->GetObjectType() == OBJECT_TYPE_BOX)
					{
						int id = obj->GetID();
						Oyster::Math::Float4x4 world = obj->GetOrientation();
						Protocol_ObjectPosition p(world, id);
						GameSession::gameSession->Send(p.GetProtocol());
					}
				}
			}
		}
		
	}

//*****************************************************//
//****************** Protocol methods *****************//
//******************************************************************************************************************//

	void GameSession::ParseProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c)
	{
		//TODO: Update response timer

		switch (p[0].value.netShort)
		{
			case protocol_Gameplay_PlayerMovement:		this->Gameplay_PlayerMovement		( Protocol_PlayerMovement		(p), c );
			break;
			case protocol_Gameplay_PlayerLookDir:		this->Gameplay_PlayerLookDir		( Protocol_PlayerLook			(p), c );
			break;
			case protocol_Gameplay_PlayerChangeWeapon:	this->Gameplay_PlayerChangeWeapon	( Protocol_PlayerChangeWeapon	(p), c );
			break;
			case protocol_Gameplay_PlayerShot:			this->Gameplay_PlayerShot			( Protocol_PlayerShot			(p), c );
			break;
			case protocol_Gameplay_PlayerJump:			this->Gameplay_PlayerJump			( Protocol_PlayerJump			(p), c );
			break;
			case protocol_Gameplay_ObjectPickup:		this->Gameplay_ObjectPickup			( Protocol_ObjectPickup			(p), c );
			break;
			case protocol_Gameplay_ObjectDamage:		this->Gameplay_ObjectDamage			( Protocol_ObjectDamage			(p), c );
			break;
			case protocol_Gameplay_ObjectPosition:		this->Gameplay_ObjectPosition		( Protocol_ObjectPosition		(p), c );
			break;
			case protocol_Gameplay_ObjectEnabled:		this->Gameplay_ObjectEnabled		( Protocol_ObjectEnable			(p), c );
			break;
			case protocol_Gameplay_ObjectDisabled:		this->Gameplay_ObjectDisabled		( Protocol_ObjectDisable		(p), c );
			break;
			case protocol_Gameplay_ObjectCreate:		this->Gameplay_ObjectCreate			( Protocol_ObjectCreate			(p), c );
			break;
			case protocol_General_Status:				this->General_Status				( Protocol_General_Status		(p), c );
			break;
			case protocol_General_Text:					this->General_Text					( Protocol_General_Text			(p), c );
			break;
		}
	}

	void GameSession::Gameplay_PlayerMovement		( Protocol_PlayerMovement& p, DanBias::GameClient* c )
	{
		if(p.bForward)		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_FORWARD);
		if(p.bBackward)		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_BACKWARD);
		if(p.bLeft)			c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_LEFT);
		if(p.bRight)		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_RIGHT);
	}
	void GameSession::Gameplay_PlayerLookDir		( Protocol_PlayerLook& p, DanBias::GameClient* c )
	{
		Oyster::Math3D::Float4 lookDir; 
		lookDir.x = p.lookDirX;
		lookDir.y = p.lookDirY;
		lookDir.z = p.lookDirZ;
		lookDir.w = p.deltaX;
		c->GetPlayer()->Rotate(lookDir);
	}
	void GameSession::Gameplay_PlayerChangeWeapon	( Protocol_PlayerChangeWeapon& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_PlayerShot			( Protocol_PlayerShot& p, DanBias::GameClient* c )
	{
		//c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_PRIMARY_PRESS);
		c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_SECONDARY_PRESS);
		//c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_PRIMARY_PRESS);
	}
	void GameSession::Gameplay_PlayerJump			( Protocol_PlayerJump& p, DanBias::GameClient* c )
	{
		if(p.hasJumped)		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_JUMP);
	}
	void GameSession::Gameplay_ObjectPickup			( Protocol_ObjectPickup& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectDamage			( Protocol_ObjectDamage& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectPosition		( Protocol_ObjectPosition& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectEnabled		( Protocol_ObjectEnable& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectDisabled		( Protocol_ObjectDisable& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectCreate			( Protocol_ObjectCreate& p, DanBias::GameClient* c )
	{

	}
	void GameSession::General_Status				( Protocol_General_Status& p, DanBias::GameClient* c )
	{
		switch (p.status)
		{
			case GameLogic::Protocol_General_Status::States_disconected:
				printf("Client with ID [%i] dissconnected\n", c->GetClient()->GetID());
				//TODO: Tell other clients
				//Protocol_
				this->Detach(c->GetClient()->GetID());
			break;

			case GameLogic::Protocol_General_Status::States_idle:

			break;

			case GameLogic::Protocol_General_Status::States_ready:
				c->SetReadyState(true);
			break;

			case GameLogic::Protocol_General_Status::States_leave:

			break;
		}
	}
	void GameSession::General_Text					( Protocol_General_Text& p, DanBias::GameClient* c )
	{
		printf("Message recieved from (%i):\t %s\n", c->GetID(), p.text.c_str());
	}

}//End namespace DanBias






