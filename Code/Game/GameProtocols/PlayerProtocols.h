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
		float lookdir[3];
		
		Protocol_PlayerLeftTurn()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerLeftTurn;
			this->protocol[0].type = ::Oyster::Network::NetAttributeType_Short;
			// deltaRadian
			this->protocol[1].type = ::Oyster::Network::NetAttributeType_Float;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			deltaRadian = 0.0f;
			memset(&lookdir[0], 0, sizeof(float) * 3);
		}

		Protocol_PlayerLeftTurn( const ::Oyster::Network::CustomNetProtocol &p )
		{
			this->deltaRadian = p[1].value.netFloat;
			lookdir[0] = p[2].value.netFloat;
			lookdir[1] = p[3].value.netFloat;
			lookdir[2] = p[4].value.netFloat;
		}

		Protocol_PlayerLeftTurn( float deltaRadian,float v[3] )
		{
			this->protocol[0].value = protocol_Gameplay_PlayerLeftTurn;
			this->protocol[0].type = ::Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = ::Oyster::Network::NetAttributeType_Float;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->deltaRadian = deltaRadian;
			memcpy(&lookdir[0], &v[0], sizeof(float) * 3);

		}
		
		::Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->deltaRadian;
			this->protocol[2].value = this->lookdir[0];
			this->protocol[3].value = this->lookdir[1];
			this->protocol[4].value = this->lookdir[2];
			return protocol;
		}

		private:
		::Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_PlayerChangeWeapon :public Oyster::Network::CustomProtocolObject
	{
		
		short weaponId;

		Protocol_PlayerChangeWeapon()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerChangeWeapon;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			
			weaponId = -1;
		}
		Protocol_PlayerChangeWeapon(Oyster::Network::CustomNetProtocol& p)
		{
			weaponId = p[1].value.netShort;
		}
		Protocol_PlayerChangeWeapon(int weaponID)
		{
			this->protocol[0].value = protocol_Gameplay_PlayerChangeWeapon;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			
			weaponId = weaponID;

		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = weaponId;
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_PlayerShot :public Oyster::Network::CustomProtocolObject
	{
		enum ShootValue
		{
			ShootValue_PrimaryPress,
			ShootValue_PrimaryRelease,
			ShootValue_SecondaryPress,
			ShootValue_SecondaryRelease,
			ShootValue_UtilityPress,
			ShootValue_UtilityRelease,
			ShootValue_DropItem,
		} value;
		//bool primaryPressed; 
		//bool secondaryPressed;
		//bool utilityPressed; 

		Protocol_PlayerShot()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerShot;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Char;

			//this->protocol[1].type = Oyster::Network::NetAttributeType_Bool;
			//this->protocol[2].type = Oyster::Network::NetAttributeType_Bool;
			//this->protocol[3].type = Oyster::Network::NetAttributeType_Bool;
		}
		Protocol_PlayerShot(ShootValue val)
		{
			this->protocol[0].value = protocol_Gameplay_PlayerShot;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Char;
			this->value = val;
		}
		Protocol_PlayerShot(Oyster::Network::CustomNetProtocol& p)
		{
			value = (ShootValue)p[1].value.netChar;
			//primaryPressed		= p[1].value.netBool;
			//secondaryPressed	= p[2].value.netBool;
			//utilityPressed		= p[3].value.netBool;
		}
		const Protocol_PlayerShot& operator=(Oyster::Network::CustomNetProtocol& val)
		{
			value = (ShootValue)val[1].value.netChar;
			//primaryPressed		= val[1].value.netBool;
			//secondaryPressed	= val[2].value.netBool;
			//utilityPressed		= val[3].value.netBool;
			return *this;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = value;
			//this->protocol[1].value = primaryPressed;
			//this->protocol[2].value = secondaryPressed;
			//this->protocol[3].value = utilityPressed;
			return protocol;
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
	//#define protocol_Gameplay_ObjectPickup			350
	struct Protocol_PlayerScore :public Oyster::Network::CustomProtocolObject
	{
		int playerID;
		int killCount;
		int deathCount;

		Protocol_PlayerScore()
		{
			this->protocol[0].value = protocol_Gameplay_PlayerScore;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Int;

			playerID	= -1;
			killCount	= -1;
			deathCount	= -1;
		}
		Protocol_PlayerScore(Oyster::Network::CustomNetProtocol& p)
		{
			playerID = p[1].value.netShort;
			killCount = p[2].value.netInt;
			deathCount = p[3].value.netInt;
		}
		Protocol_PlayerScore(int objectID, int kills, int deaths)
		{
			this->protocol[0].value = protocol_Gameplay_PlayerScore;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Int;

			playerID	= objectID;
			killCount	= kills;
			deathCount	= deaths;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = playerID;
			this->protocol[2].value = killCount;
			this->protocol[3].value = deathCount;
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
