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

	//protocol_Gameplay_PlayerLeftTurn
	struct Protocol_PlayerLeftTurn : public ::Oyster::Network::CustomProtocolObject
	{
	public:
		float deltaRadian;
		
		Protocol_PlayerLeftTurn()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerLeftTurn;
			this->protocol[0].type = ::Oyster::Network::NetAttributeType_Short;
			// deltaRadian
			this->protocol[1].type = ::Oyster::Network::NetAttributeType_Float;
		}

		Protocol_PlayerLeftTurn( const ::Oyster::Network::CustomNetProtocol &p )
		{
			this->deltaRadian = p[1].value.netFloat;
		}

		Protocol_PlayerLeftTurn( float deltaRadian )
		{
			this->protocol[0].value = protocol_Gameplay_PlayerLeftTurn;
			this->protocol[0].type = ::Oyster::Network::NetAttributeType_Short;
			// deltaRadian
			this->protocol[1].type = ::Oyster::Network::NetAttributeType_Float;
			this->deltaRadian = deltaRadian;
		}
		
		::Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->deltaRadian;
			return protocol;
		}

		private:
		::Oyster::Network::CustomNetProtocol protocol;
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
