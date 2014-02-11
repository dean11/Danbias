/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\GameSession.h"
#include "..\GameClient.h"
#include <WinTimer.h>

#include <Protocols.h>
#include <PostBox\PostBox.h>
#include <GameLogicStates.h>
#include <OysterMath.h>

#define NOMINMAX
#include <Windows.h>


using namespace Utility::DynamicMemory;
using namespace Oyster;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace GameLogic;

namespace DanBias
{
	Utility::WinTimer testTimer;
	int testID = -1;

	bool GameSession::DoWork(  )
	{
		if(this->isRunning)
		{
			float dt = (float)this->logicTimer.getElapsedSeconds();
			if( dt >= this->logicFrameTime )
			{
				this->ProcessClients();
				this->gameInstance.NewFrame();

				this->logicTimer.reset();
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
				printf("\t(%i : %s) - EventType_ProtocolFailedToSend\n", cl->GetClient()->GetID(), e.sender->GetIpAddress().c_str());	
				this->Detach(e.sender);
			break;
			case NetworkClient::ClientEventArgs::EventType_ProtocolRecieved:
				printf("\t(%i : %s) - EventType_ProtocolRecieved\n", cl->GetClient()->GetID(), e.sender->GetIpAddress().c_str());	
				testID = 2;
				if(cl->GetPlayer()->GetID() == testID)//TODO: TEST
				{
					testTimer.reset();
				}
				this->ParseProtocol(e.args.data.protocol, cl);
			break;
		}
	}

	void GameSession::ObjectMove(GameLogic::IObjectData* movedObject)
	{
		float dt = GameSession::gameSession->networkTimer.getElapsedSeconds();
		//Duh... This was causing alot of problems, it's in the wrong place...
		//Need to figure out where to put this frame locker.
		//We only need to send network packages when necessary, ie not 120 times per frame. 
		//I think it would be enough with 60-70 packages per second due to the nature of
		//graphics update (60 fps) on the client side. To send more than this would be lost
		//bandwidth.
		//if( dt >= GameSession::gameSession->networkFrameTime )
		{
			GameSession::gameSession->networkTimer.reset();

			GameLogic::IObjectData* obj = movedObject;

			int id = obj->GetID();
			Protocol_ObjectPosition p(obj->GetOrientation(), id);
			GameSession::gameSession->Send(p.GetProtocol());
		}
		
	}
	void GameSession::ObjectDisabled( GameLogic::IObjectData* movedObject, float seconds )
	{
		GameSession::gameSession->Send(Protocol_ObjectDisable(movedObject->GetID(), seconds).GetProtocol());
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
		if(p.secondaryPressed)	c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_SECONDARY_PRESS);
		if(p.primaryPressed)	c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_PRIMARY_PRESS);
		
		if(p.utilityPressed)	c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_UTILLITY_PRESS);
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
		printf("Message recieved from (%i):\t %s\n", c->GetClient()->GetID(), p.text.c_str());
	}

}//End namespace DanBias






