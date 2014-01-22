//////////////////////////////////////////////////////////
// Created 2013											//
// Dennis Andersen, Linda Andersson						//
//////////////////////////////////////////////////////////

#ifndef GAMELOGIC_PLAYER_PROTOCOLS_H
#define GAMELOGIC_PLAYER_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"
#include <bitset>

//protocol_Gameplay_PlayerMovement			300
//protocol_Gameplay_PlayerMouseMovement		301
//protocol_Gameplay_PlayerChangeWeapon		302

namespace GameLogic
{
	struct Protocol_PlayerMovement :public Oyster::Network::CustomProtocolObject
	{
		bool bForward;
		bool bBackward;
		bool bLeft;
		bool bRight;
		
		Protocol_PlayerMovement()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_PlayerMovement;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Bool;
		}
		const Protocol_PlayerMovement& operator=(Oyster::Network::CustomNetProtocol& val)
		{
			bForward		= val[1].value.netBool;
			bBackward		= val[2].value.netBool;
			bLeft			= val[3].value.netBool;
			bRight			= val[4].value.netBool;

			return *this;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value = bForward;
			this->protocol[2].value = bBackward;
			this->protocol[3].value = bLeft;
			this->protocol[4].value = bRight;

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
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_PlayerMouseMovement;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			
		}
		const Protocol_PlayerMouse& operator=(Oyster::Network::CustomNetProtocol& val)
		{
			dxMouse		= val[1].value.netFloat;
			dyMouse		= val[2].value.netFloat;

			return *this;
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

	struct Protocol_PlayerChangeWeapon :public Oyster::Network::CustomProtocolObject
	{
		
		int ID;

		Protocol_PlayerChangeWeapon()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_PlayerChangeWeapon;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			
		}
		const Protocol_PlayerChangeWeapon& operator=(Oyster::Network::CustomNetProtocol& val)
		{
			return *this;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			return &protocol;
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
