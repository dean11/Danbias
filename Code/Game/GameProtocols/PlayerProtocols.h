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

		bool bForward;
		bool bBackward;
		bool bTurnLeft;
		bool bTurnRight;
		bool bStrafeRight;
		bool bStrafeLeft;
		
		Protocol_PlayerMovement()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerNavigation;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

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

	struct Protocol_PlayerMouse :public Oyster::Network::CustomProtocolObject
	{

		float dxMouse;
		float dyMouse;
		

		Protocol_PlayerMouse()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerMouseMovement;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value = dxMouse;
			this->protocol[2].value = dyMouse;

			return &protocol;
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_PlayerPosition :public Oyster::Network::CustomProtocolObject
	{
		
		float position[3];
		// look at dir 

		Protocol_PlayerPosition()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerPosition;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value = position[0];
			this->protocol[2].value = position[1];
			this->protocol[3].value = position[2];
			return &protocol;
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
