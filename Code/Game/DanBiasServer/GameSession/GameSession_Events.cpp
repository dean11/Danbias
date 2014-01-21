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
			case protocol_Gameplay_PlayerMouseMovement:
			{
				Protocol_PlayerMouse m; m = p;
				c->GetPlayer()->Rotate(m.dxMouse, m.dyMouse);
			}
			break;
			case protocol_Gameplay_PlayerChangeWeapon:
				
			break;
			case protocol_Gameplay_PlayerDamage:

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
						this->RemoveClient(c);
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


	void GameSession::ObjectMove(GameLogic::IObjectData* movedObject)
	{
		movedObject->GetID();
		movedObject->GetOrientation();

	}

}//End namespace DanBias

