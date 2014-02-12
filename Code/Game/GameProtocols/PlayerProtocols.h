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
	struct Protocol_PlayerMovementRight :public Oyster::Network::CustomProtocolObject
	{
		Protocol_PlayerMovementRight()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerMovementRight;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{ return protocol; }

		private:
			Oyster::Network::CustomNetProtocol protocol;
	};
	struct Protocol_PlayerMovementLeft :public Oyster::Network::CustomProtocolObject
	{
		Protocol_PlayerMovementLeft()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerMovementLeft;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override { return protocol; }

		private:
			Oyster::Network::CustomNetProtocol protocol;
	};
	struct Protocol_PlayerMovementForward :public Oyster::Network::CustomProtocolObject
	{
		Protocol_PlayerMovementForward()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerMovementForward;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override { return protocol; }

		private:
			Oyster::Network::CustomNetProtocol protocol;
	};
	struct Protocol_PlayerMovementBackward :public Oyster::Network::CustomProtocolObject
	{
		Protocol_PlayerMovementBackward()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerMovementBackward;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override { return protocol; }

		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_PlayerJump :public Oyster::Network::CustomProtocolObject
	{
		Protocol_PlayerJump()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerJump;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override { return protocol; }

		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_PlayerLook :public Oyster::Network::CustomProtocolObject
	{
		// can be swapped to a quaternion later
		float lookDir[3]; 
		float right[3]; 

		Protocol_PlayerLook()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerLookDir;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			// LookDir
			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			// Right
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;

			memset(&this->lookDir[0], 0, sizeof(float) * 3);
			memset(&this->right[0], 0, sizeof(float) * 3);
		}
		Protocol_PlayerLook(Oyster::Network::CustomNetProtocol& p)
		{
			this->lookDir[0] = p[1].value.netFloat;
			this->lookDir[1] = p[2].value.netFloat;
			this->lookDir[2] = p[3].value.netFloat;

			this->right[0] = p[4].value.netFloat;
			this->right[1] = p[5].value.netFloat;
			this->right[2] = p[6].value.netFloat;
		}
		Protocol_PlayerLook(float l[3], float r[3])
		{
			this->protocol[0].value = protocol_Gameplay_PlayerLookDir;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;

			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;

			memcpy(&this->lookDir[0], &l[0], sizeof(float) * 3);
			memcpy(&this->right[0], &r[0], sizeof(float) * 3);
		}
		
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->lookDir[0];
			this->protocol[2].value = this->lookDir[1];
			this->protocol[3].value = this->lookDir[2];
			this->protocol[4].value = this->right[0];
			this->protocol[5].value = this->right[1];
			this->protocol[6].value = this->right[2];	

			return protocol;
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_PlayerChangeWeapon :public Oyster::Network::CustomProtocolObject
	{
		
		int ID;

		Protocol_PlayerChangeWeapon()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerChangeWeapon;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_PlayerChangeWeapon(Oyster::Network::CustomNetProtocol& p)
		{

		}
		const Protocol_PlayerChangeWeapon& operator=(Oyster::Network::CustomNetProtocol& val)
		{
			return *this;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			return protocol;
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_PlayerShot :public Oyster::Network::CustomProtocolObject
	{
		bool primaryPressed; 
		bool secondaryPressed;
		bool utilityPressed; 

		Protocol_PlayerShot()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerShot;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Bool;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Bool;
		}
		Protocol_PlayerShot(Oyster::Network::CustomNetProtocol& p)
		{
			primaryPressed		= p[1].value.netBool;
			secondaryPressed	= p[2].value.netBool;
			utilityPressed		= p[3].value.netBool;
		}
		const Protocol_PlayerShot& operator=(Oyster::Network::CustomNetProtocol& val)
		{
			primaryPressed		= val[1].value.netBool;
			secondaryPressed	= val[2].value.netBool;
			utilityPressed		= val[3].value.netBool;
			return *this;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = primaryPressed;
			this->protocol[2].value = secondaryPressed;
			this->protocol[3].value = utilityPressed;
			return protocol;
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
