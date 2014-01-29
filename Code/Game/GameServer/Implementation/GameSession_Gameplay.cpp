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
			double dt = this->timer.getElapsedSeconds();
			gameInstance.SetFrameTimeLength((float)dt);

			if(dt >= DELTA_TIME_20)
			{
				this->ProcessClients();

				this->gameInstance.NewFrame();

				this->timer.reset();
			}
		}

		return this->isRunning;
	}

	void GameSession::ClientEventCallback(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e)
	{

	}

	void GameSession::ObjectMove(GameLogic::IObjectData* movedObject)
	{
		//GameLogic::IObjectData* obj = NULL;
		//if(dynamic_cast<GameLogic::ILevelData*>(movedObject))
		//	obj =((GameLogic::ILevelData*)movedObject)->GetObjectAt(0);
		//if(obj)
		//{
		//	if(obj->GetType() == OBJECT_TYPE_BOX)
		//	{
		//		obj->GetID();
		//		Oyster::Math::Float4x4 world =obj->GetOrientation();
		//		Protocol_ObjectPosition p(world, 1);
		//		GameSession::gameSession->Send(p.GetProtocol());
		//	}
		//}
		
	}

//*****************************************************//
//****************** Protocol methods *****************//
//******************************************************************************************************************//

	void GameSession::ParseProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c)
	{
		switch (p[protocol_INDEX_ID].value.netShort)
		{
			case protocol_Gameplay_PlayerMovement:		this->Gameplay_PlayerMovement		( Protocol_PlayerMovement		(p), c );
			break;
			case protocol_Gameplay_PlayerLookDir:		this->Gameplay_PlayerLookDir		( Protocol_PlayerLook			(p), c );
			break;
			case protocol_Gameplay_PlayerChangeWeapon:	this->Gameplay_PlayerChangeWeapon	( Protocol_PlayerChangeWeapon	(p), c );
			break;
			case protocol_Gameplay_PlayerShot:			this->Gameplay_PlayerShot			( Protocol_PlayerShot			(p), c );
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
		Oyster::Math3D::Float3 lookDir; 
		lookDir.x = p.lookDirX;
		lookDir.y = p.lookDirY;
		lookDir.z = p.lookDirZ;
		c->GetPlayer()->Rotate(lookDir);
	}
	void GameSession::Gameplay_PlayerChangeWeapon	( Protocol_PlayerChangeWeapon& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_PlayerShot			( Protocol_PlayerShot& p, DanBias::GameClient* c )
	{
		c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_PRIMARY_PRESS);
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
				this->Detach(c->GetClient()->GetID());
			break;

			case GameLogic::Protocol_General_Status::States_idle:

			break;

			case GameLogic::Protocol_General_Status::States_ready:

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






