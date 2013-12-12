//////////////////////////////////////////////////////////
// Created 2013											//
// Dennis Andersen, Linda Andersson						//
//////////////////////////////////////////////////////////

#ifndef GAMELOGIC_PLAYER_PROTOCOLS_H
#define GAMELOGIC_PLAYER_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"



namespace GameLogic
{
	struct Protocol_PlayerMovement :public Oyster::Network::CustomProtocolObject
	{
		int ProtocolID;
		bool bForward;
		bool bBackward;
		bool bTurnLeft;
		bool bTurnRight;
		bool bStrafeRight;
		bool bStrafeLeft;
		
		Protocol_PlayerMovement()
		{
			this->protocol[0].value = ProtocolID		= protocol_PlayerNavigation;

			this->protocol[0].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[6].type = Oyster::Network::NetAttributeType_Bool;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value = bForward;
			this->protocol[2].value = bBackward;
			this->protocol[3].value = bTurnLeft;
			this->protocol[4].value = bTurnRight;
			this->protocol[5].value = bStrafeRight;
			this->protocol[6].value = bStrafeRight;

			return &protocol;
		}

		private:
			Oyster::Network::CustomNetProtocol protocol;
	};
}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
