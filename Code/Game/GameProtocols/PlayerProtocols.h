#ifndef GAMELOGIC_PLAYER_PROTOCOLS_H
#define GAMELOGIC_PLAYER_PROTOCOLS_H

#include "CustomNetProtocol.h"
#include "ProtocolIdentificationID.h"



namespace GameLogic
{
	struct Protocol_PlayerMovement :public Network::CustomProtocolObject
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
			protocol[0]->value = ProtocolID		= protocol_PlayerNavigation;

			protocol[0]->type = Network::NetAttributeType_Int;
			protocol[1]->type = Network::NetAttributeType_Bool;
			protocol[2]->type = Network::NetAttributeType_Bool;
			protocol[3]->type = Network::NetAttributeType_Bool;
			protocol[4]->type = Network::NetAttributeType_Bool;
			protocol[5]->type = Network::NetAttributeType_Bool;
			protocol[6]->type = Network::NetAttributeType_Bool;
		}
		Network::CustomNetProtocol* GetProtocol() override
		{
			protocol[1]->value = bForward;
			protocol[2]->value = bBackward;
			protocol[3]->value = bTurnLeft;
			protocol[4]->value = bTurnRight;
			protocol[5]->value = bStrafeRight;
			protocol[6]->value = bStrafeRight;

			return &protocol;
		}

		private:
			Network::CustomNetProtocol protocol;
	};
}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
