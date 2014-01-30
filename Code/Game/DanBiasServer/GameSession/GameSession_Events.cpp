/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "GameSession.h"
#include "GameClient.h"

#include <Protocols.h>
#include <PostBox\PostBox.h>
#include <GameLogicStates.h>
#include <OysterMath.h>

#include <Windows.h>

using namespace Utility::DynamicMemory;
using namespace Oyster;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace GameLogic;

namespace DanBias
{
	void GameSession::ParseEvents()
	{
		if( !this->box->IsEmpty() )
		{
			NetworkSession::NetEvent &e = this->box->Fetch();
			static int ii = 0;
			printf("%i - Message recieved! [%i]\n", ii++, e.protocol[0].value);

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
			case protocol_Gameplay_PlayerMovement:
			{
				if(p[1].value.netBool)	//bool bForward;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_FORWARD);
				if(p[2].value.netBool)	//bool bBackward;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_BACKWARD);
				if(p[3].value.netBool)	//bool bStrafeLeft;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_LEFT);
				if(p[4].value.netBool)	//bool bStrafeRight;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_RIGHT);
			}
			break;
			case protocol_Gameplay_PlayerLookDir:
			{
				Protocol_PlayerLook m; m = p;
				Oyster::Math3D::Float3 lookDir; 
				lookDir.x = p.Get(1).value.netFloat;
				lookDir.y = p.Get(2).value.netFloat;
				lookDir.z = p.Get(3).value.netFloat;
				c->GetPlayer()->Rotate(lookDir);
			}
			break;
			case protocol_Gameplay_PlayerChangeWeapon:
				
			break;
			case protocol_Gameplay_PlayerShot:
				if (p[1].value.netBool)
					c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_PRIMARY_PRESS);
				break;
			case protocol_Gameplay_PlayerJump:
				if (p[1].value.netBool)
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_JUMP);
				break;
			case protocol_Gameplay_ObjectDamage:

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
					case GameLogic::Protocol_General_Status::States_disconected:
						printf("Client with ID [%i] dissconnected\n", c->GetClient()->GetID());
						this->RemoveClient(c);
					break;

					case GameLogic::Protocol_General_Status::States_idle:

					break;

					case GameLogic::Protocol_General_Status::States_ready:

					break;

					case GameLogic::Protocol_General_Status::States_leave:

					break;
				}
			break;
			case protocol_General_Text:
			{
				GameLogic::Protocol_General_Text temp(p);
				printf("Message recieved from (%i):\t %s\n", c->GetID(), temp.text.c_str());
			}
			break;

		}
	}


	void GameSession::ObjectMove(GameLogic::IObjectData* movedObject)
	{
		GameLogic::IObjectData* obj = NULL;
		if(dynamic_cast<GameLogic::ILevelData*>(movedObject))
		{
			obj =((GameLogic::ILevelData*)movedObject)->GetObjectAt(0);
			if(obj)
			{
				if(obj->GetType() == OBJECT_TYPE_WORLD)
				{
					obj->GetID();
					Oyster::Math::Float4x4 world =obj->GetOrientation();
					Oyster::Math3D::Float4x4 scale = Oyster::Math3D::Float4x4::identity;
					scale.v[0].x = 8;
					scale.v[1].y = 8;
					scale.v[2].z = 8;
					//world = world * scale;
					Protocol_ObjectPosition p(world, 0);
					GameSession::gameSession->Send(p.GetProtocol());
				}
			}
			obj = NULL;
			obj =((GameLogic::ILevelData*)movedObject)->GetObjectAt(1);
			if(obj)
			{
				if(obj->GetType() == OBJECT_TYPE_BOX)
				{
					obj->GetID();
					Oyster::Math::Float4x4 world = obj->GetOrientation();
					Protocol_ObjectPosition p(world, 1);
					GameSession::gameSession->Send(p.GetProtocol());
				}
			}
		}
		
	}

}//End namespace DanBias

