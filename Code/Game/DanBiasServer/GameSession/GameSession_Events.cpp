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
			case protocol_Gameplay_PlayerNavigation:
			{
				
				//Oyster::Math::Float4x4 world = Oyster::Math::Matrix::identity;
				if(p[1].value.netBool)	//bool bForward;
					//world.v[3].x = 2;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_FORWARD);
				if(p[2].value.netBool)	//bool bBackward;
					//world.v[3].x = -2;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_BACKWARD);
				if(p[5].value.netBool)	//bool bStrafeRight;
					//world.v[3].y = 2;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_RIGHT);
				if(p[6].value.netBool)	//bool bStrafeLeft;
					//world.v[3].y = -2;
					c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_LEFT);
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
		//if (movedObject->)
		//{
		//
		//}
		//movedObject->GetOrientation();
	}

}//End namespace DanBias

