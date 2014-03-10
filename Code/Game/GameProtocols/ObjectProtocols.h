//////////////////////////////////////////////////////////
// Created 2013											//
// Dennis Andersen, Linda Andersson						//
//////////////////////////////////////////////////////////

#ifndef GAMELOGIC_OBJECT_PROTOCOLS_H
#define GAMELOGIC_OBJECT_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"

namespace GameLogic
{
	//#define protocol_Gameplay_ObjectPickup			350
	struct Protocol_ObjectPickup :public Oyster::Network::CustomProtocolObject
	{
		short object_ID;
		short pickup_ID;

		Protocol_ObjectPickup()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectPickup;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Short;
			
			object_ID = -1;
			pickup_ID = -1;
		}
		Protocol_ObjectPickup(Oyster::Network::CustomNetProtocol& p)
		{
			object_ID = p[1].value.netShort;
			pickup_ID = p[2].value.netShort;
		}
		Protocol_ObjectPickup(int objectID, short pickupID)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectPickup;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Short;
			
			object_ID = objectID;
			pickup_ID = pickupID;

		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = object_ID;
			this->protocol[2].value = pickup_ID;
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectDamage			351
	struct Protocol_ObjectDamage :public Oyster::Network::CustomProtocolObject
	{
		int objectID;
		float health; 
		int eventID;

		Protocol_ObjectDamage()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectDamage;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Int;
			
			this->objectID = -1;
			this->health = 0.0f;
			this->eventID = -1;
		}
		Protocol_ObjectDamage(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt; 
			this->health = p[2].value.netFloat; 
			this->eventID = p[3].value.netInt;
		}
		Protocol_ObjectDamage(int id, float hp, int eventID)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectDamage;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
					
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Int;

			this->objectID = id;
			this->health = hp;
			this->eventID = eventID;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = objectID;
			this->protocol[2].value = health;
			this->protocol[3].value = eventID;
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectHealthStatus		352
	struct Protocol_ObjectHealthStatus :public Oyster::Network::CustomProtocolObject
	{
		int objectID;
		float currentHealth;

		Protocol_ObjectHealthStatus()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectHealthStatus;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->objectID = -1; 
			this->currentHealth = 0.0f; 
		}
		Protocol_ObjectHealthStatus(int id, float health)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectHealthStatus;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->objectID = id; this->currentHealth = health; 
		}
		Protocol_ObjectHealthStatus(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt; 
			this->currentHealth = p[2].value.netFloat; 
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			this->protocol[2].value = this->currentHealth;
			
			return protocol;		 
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectPosition			353
	struct Protocol_ObjectPosition :public Oyster::Network::CustomProtocolObject
	{
		short objectID;
		float position[3]; 
		
		Protocol_ObjectPosition()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectPosition;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;		
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			
			objectID = -1;
			memset(&position[0], 0, sizeof(float) * 3);
		}
		Protocol_ObjectPosition(Oyster::Network::CustomNetProtocol& p)
		{
			objectID = p[1].value.netShort;
			position[0] = p[2].value.netFloat;
			position[1] = p[3].value.netFloat;
			position[2] = p[4].value.netFloat;
		}
		Protocol_ObjectPosition(float v[3], int id)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectPosition;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;

			objectID = id;
			memcpy(&position[0], &v[0], sizeof(float) * 3);
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = objectID;
			this->protocol[2].value = position[0];
			this->protocol[3].value = position[1];
			this->protocol[4].value = position[2];
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectScale				354
	struct Protocol_ObjectScale :public Oyster::Network::CustomProtocolObject
	{
		short objectID;
		float scale[3]; 
		
		Protocol_ObjectScale()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectScale;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;		
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			
			objectID = -1;
			memset(&scale[0], 0, sizeof(float) * 3);
		}
		Protocol_ObjectScale(Oyster::Network::CustomNetProtocol& p)
		{
			objectID = p[1].value.netShort;
			scale[0] = p[2].value.netFloat;
			scale[1] = p[3].value.netFloat;
			scale[2] = p[4].value.netFloat;
		}
		Protocol_ObjectScale(float v[3], int id)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectScale;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;

			objectID = id;
			memcpy(&scale[0], &v[0], sizeof(float) * 3);
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = objectID;
			this->protocol[2].value = scale[0];
			this->protocol[3].value = scale[1];
			this->protocol[4].value = scale[2];
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectRotation			355
	struct Protocol_ObjectRotation :public Oyster::Network::CustomProtocolObject
	{
		short objectID;
		float rotationQ[4]; 
		
		Protocol_ObjectRotation()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectRotation;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;		
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			
			objectID = -1;
			memset(&rotationQ[0], 0, sizeof(float) * 4);
		}
		Protocol_ObjectRotation(Oyster::Network::CustomNetProtocol& p)
		{
			objectID = p[1].value.netShort;
			rotationQ[0] = p[2].value.netFloat;
			rotationQ[1] = p[3].value.netFloat;
			rotationQ[2] = p[4].value.netFloat;
			rotationQ[3] = p[5].value.netFloat;
		}
		Protocol_ObjectRotation(float v[4], int id)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectRotation;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;

			objectID = id;
			memcpy(&rotationQ[0], &v[0], sizeof(float) * 4);
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = objectID;
			this->protocol[2].value = rotationQ[0];
			this->protocol[3].value = rotationQ[1];
			this->protocol[4].value = rotationQ[2];
			this->protocol[5].value = rotationQ[3];
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
	
	//#define protocol_Gameplay_ObjectEnabled			356
	struct Protocol_ObjectPositionRotation :public Oyster::Network::CustomProtocolObject
	{
		short objectID;
		float position[3]; 
		float rotationQ[4]; 
		
		Protocol_ObjectPositionRotation()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectPositionRotation;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;		
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		//POSITION	
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
		//ROTATION
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[7].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[8].type = Oyster::Network::NetAttributeType_Float;

			this->objectID = -1;
			memset(&this->position[0], 0, sizeof(float) * 3);
			memset(&this->rotationQ[0], 0, sizeof(float) * 4);
		}
		Protocol_ObjectPositionRotation(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netShort;
		//POSITION
			this->position[0] = p[2].value.netFloat;
			this->position[1] = p[3].value.netFloat;
			this->position[2] = p[4].value.netFloat;
		//ROTATION
			this->rotationQ[0] = p[5].value.netFloat;
			this->rotationQ[1] = p[6].value.netFloat;
			this->rotationQ[2] = p[7].value.netFloat;
			this->rotationQ[3] = p[8].value.netFloat;
		}
		Protocol_ObjectPositionRotation(float p[3], float r[4], int id)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectPositionRotation;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		//POSITION
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
		//ROTATION
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[7].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[8].type = Oyster::Network::NetAttributeType_Float;

			objectID = id;
			memcpy(&this->position[0], &p[0], sizeof(float) * 3);
			memcpy(&this->rotationQ[0], &r[0], sizeof(float) * 4);
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			this->protocol[2].value = this->position[0];
			this->protocol[3].value = this->position[1];
			this->protocol[4].value = this->position[2];
			this->protocol[5].value = this->rotationQ[0];
			this->protocol[6].value = this->rotationQ[1];
			this->protocol[7].value = this->rotationQ[2];
			this->protocol[8].value = this->rotationQ[3];
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectEnabled			357
	struct Protocol_ObjectEnable :public Oyster::Network::CustomProtocolObject
	{
		int objectID;

		Protocol_ObjectEnable()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectEnabled;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;				
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = -1;
		}
		Protocol_ObjectEnable(int objectID)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectEnabled;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;				
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = objectID;
		}
		Protocol_ObjectEnable(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectDisabled			358
	struct Protocol_ObjectDisable :public Oyster::Network::CustomProtocolObject
	{
		int objectID;

		Protocol_ObjectDisable()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectDisabled;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = -1;
		}
		Protocol_ObjectDisable(int objctID)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectDisabled;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = objctID;
		}
		Protocol_ObjectDisable(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectCreate			359
	struct Protocol_ObjectCreate :public Oyster::Network::CustomProtocolObject
	{
		//ObjectType type; //ie player, box or whatever
		int objectID;
		std::string name;
		float position[3];
		float rotationQ[4];
		float scale[3];

		Protocol_ObjectCreate()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectCreate;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		//NAME
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_CharArray;	
		//POSITION	
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
		//ROTATION
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[7].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[8].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[9].type = Oyster::Network::NetAttributeType_Float;
		//SCALE
			this->protocol[10].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[11].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[12].type = Oyster::Network::NetAttributeType_Float;

			this->objectID = -1;
			memset(this->position, 0, sizeof(float) * 3);
			memset(this->rotationQ, 0, sizeof(float) * 4);
		}
		Protocol_ObjectCreate( Oyster::Network::CustomNetProtocol& p )
		{
			this->objectID = p[1].value.netInt;
			this->name.assign(p[2].value.netCharPtr);
			
			this->position[0] = p[3].value.netFloat;
			this->position[1] = p[4].value.netFloat;
			this->position[2] = p[5].value.netFloat;

			this->rotationQ[0] = p[6].value.netFloat;
			this->rotationQ[1] = p[7].value.netFloat;
			this->rotationQ[2] = p[8].value.netFloat;
			this->rotationQ[3] = p[9].value.netFloat;

			this->scale[0] = p[10].value.netFloat;
			this->scale[1] = p[11].value.netFloat;
			this->scale[2] = p[12].value.netFloat;
		}
		Protocol_ObjectCreate(float p[3], float r[4], float s[3], int id, char *path)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectCreate;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Int;
		//NAME							
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_CharArray; 
		//POSITION	
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
		//ROTATION
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[7].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[8].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[9].type = Oyster::Network::NetAttributeType_Float;
		//SCALE
			this->protocol[10].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[11].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[12].type = Oyster::Network::NetAttributeType_Float;

			objectID = id;
			this->name = path;

			memcpy(this->position, p, sizeof(float) * 3);
			memcpy(this->rotationQ, r, sizeof(float) * 4);
			memcpy(this->scale, s, sizeof(float) * 3);
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{

			this->protocol[1].value = objectID;
			this->protocol.Set(2, name);
			this->protocol[3].value = this->position[0];
			this->protocol[4].value = this->position[1];
			this->protocol[5].value = this->position[2];
			this->protocol[6].value = this->rotationQ[0];
			this->protocol[7].value = this->rotationQ[1];
			this->protocol[8].value = this->rotationQ[2];
			this->protocol[9].value = this->rotationQ[3];
			this->protocol[10].value = this->scale[0];
			this->protocol[11].value = this->scale[1];
			this->protocol[12].value = this->scale[2];

			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectDelete			360
	struct Protocol_ObjectDelete :public Oyster::Network::CustomProtocolObject
	{
		int objectID;

		Protocol_ObjectDelete()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectDelete;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = -1;
		}
		Protocol_ObjectDelete(int objctID)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectDelete;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = objctID;
		}
		Protocol_ObjectDelete(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectCreatePlayer		361
	struct Protocol_ObjectCreatePlayer :public Oyster::Network::CustomProtocolObject
	{
		/*1*/			int objectID;
		/*2*/			int teamId;
		/*3*/			bool owner;
		/*4*/			std::string name;
		/*5*/			std::string meshName;
		/*6 - 8*/		float position[3];
		/*9 - 12*/		float rotationQ[4];
		/*13 - 15*/		float scale[3];

		Protocol_ObjectCreatePlayer()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectCreatePlayer;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

		//PLAYER_ID
			this->protocol[1].type = Oyster::Network::NetAttributeType_Bool;
		//TEAM_ID
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
		//OWNER
			this->protocol[3].type = Oyster::Network::NetAttributeType_Int;
		//PLAYER-NAME
			this->protocol[4].type = Oyster::Network::NetAttributeType_CharArray;
		//MESH-NAME
			this->protocol[5].type = Oyster::Network::NetAttributeType_CharArray;
		//POSITION
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[7].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[8].type = Oyster::Network::NetAttributeType_Float;
		//ROTATION
			this->protocol[9].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[10].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[11].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[12].type = Oyster::Network::NetAttributeType_Float;
		//SCALE
			this->protocol[13].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[14].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[15].type = Oyster::Network::NetAttributeType_Float;
		}
		Protocol_ObjectCreatePlayer(Oyster::Network::CustomNetProtocol& p)
		{
			this->owner = p[1].value.netBool;
			this->objectID = p[2].value.netInt;
			this->teamId = p[3].value.netInt;

			this->name.assign(p[4].value.netCharPtr);
			this->meshName.assign(p[5].value.netCharPtr);
			
			this->position[0] = p[6].value.netFloat;
			this->position[1] = p[7].value.netFloat;
			this->position[2] = p[8].value.netFloat;

			this->rotationQ[0] = p[9].value.netFloat;
			this->rotationQ[1] = p[10].value.netFloat;
			this->rotationQ[2] = p[11].value.netFloat;
			this->rotationQ[3] = p[12].value.netFloat;

			this->scale[0] = p[13].value.netFloat;
			this->scale[1] = p[14].value.netFloat;
			this->scale[2] = p[15].value.netFloat;
		}
		Protocol_ObjectCreatePlayer(float position[3], float rotation[4], float scale[3], int ObjectID, bool owner, int teamID, std::string name, std::string meshName)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectCreatePlayer;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

		//PLAYER_ID
			this->protocol[1].type = Oyster::Network::NetAttributeType_Bool;
		//TEAM_ID
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
		//OWNER
			this->protocol[3].type = Oyster::Network::NetAttributeType_Int;
		//PLAYER-NAME
			this->protocol[4].type = Oyster::Network::NetAttributeType_CharArray;
		//MESH-NAME
			this->protocol[5].type = Oyster::Network::NetAttributeType_CharArray;
		//POSITION
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[7].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[8].type = Oyster::Network::NetAttributeType_Float;
		//ROTATION
			this->protocol[9].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[10].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[11].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[12].type = Oyster::Network::NetAttributeType_Float;
		//SCALE
			this->protocol[13].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[14].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[15].type = Oyster::Network::NetAttributeType_Float;

			this->objectID = ObjectID;
			this->teamId = teamID;
			this->owner = owner;
			this->name = name;
			this->meshName = meshName;
			memcpy(&this->position[0], &position[0], sizeof(float)*3);
			memcpy(&this->rotationQ[0], &rotation[0], sizeof(float)*4);
			memcpy(&this->scale[0], &scale[0], sizeof(float)*3);
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->owner;
			this->protocol[2].value = this->objectID;
			this->protocol[3].value = this->teamId;
			
			this->protocol.Set(4, this->name);
			this->protocol.Set(5, this->meshName);

		//POSITION
			this->protocol[6].value = this->position[0];
			this->protocol[7].value = this->position[1];
			this->protocol[8].value = this->position[2];
		//ROTATION
			this->protocol[9].value  = this->rotationQ[0];
			this->protocol[10].value  = this->rotationQ[1];
			this->protocol[11].value = this->rotationQ[2];
			this->protocol[12].value = this->rotationQ[3];
		//SCALE
			this->protocol[13].value = this->scale[0];
			this->protocol[14].value = this->scale[1];
			this->protocol[15].value = this->scale[2];

			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectJoinTeam			362
	struct Protocol_ObjectJoinTeam :public Oyster::Network::CustomProtocolObject
	{
		int objectID;
		int teamID;

		Protocol_ObjectJoinTeam()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectJoinTeam;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = 0;
			this->teamID = 0;
		}
		Protocol_ObjectJoinTeam(int objID, int teamID)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectJoinTeam;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = objID;
			this->teamID = teamID;
		}
		Protocol_ObjectJoinTeam(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt;
			this->teamID = p[2].value.netInt;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			this->protocol[2].value = this->teamID;
			return protocol;
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectLeaveTeam			363
	struct Protocol_ObjectLeaveTeam :public Oyster::Network::CustomProtocolObject
	{
		int objectID;
		Protocol_ObjectLeaveTeam()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectLeaveTeam;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = 0;
		}
		Protocol_ObjectLeaveTeam(int objectID)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectLeaveTeam;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = objectID;
		}
		Protocol_ObjectLeaveTeam(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			return protocol;		 
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectWeaponCooldown	364
	struct Protocol_ObjectWeaponCooldown :public Oyster::Network::CustomProtocolObject
	{
		float seconds;
		Protocol_ObjectWeaponCooldown()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectWeaponCooldown;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->seconds = 0.0f;
		}
		Protocol_ObjectWeaponCooldown(float seconds)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectWeaponCooldown;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->seconds = seconds;
		}
		Protocol_ObjectWeaponCooldown(Oyster::Network::CustomNetProtocol& p)
		{
			this->seconds = p[1].value.netFloat;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->seconds;
			return protocol;		 
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectWeaponEnergy		365
	struct Protocol_ObjectWeaponEnergy :public Oyster::Network::CustomProtocolObject
	{
		int objectID;
		float energy;
		Protocol_ObjectWeaponEnergy()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectWeaponEnergy;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->objectID = -1;
			this->energy = 0.0f;
		}
		Protocol_ObjectWeaponEnergy(int objectID, float energy)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectWeaponEnergy;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->objectID = objectID;
			this->energy = energy;
		}
		Protocol_ObjectWeaponEnergy(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt;
			this->energy = p[2].value.netFloat;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			this->protocol[2].value = this->energy;
			return protocol;		 
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectRespawn			366
	struct Protocol_ObjectRespawn :public Oyster::Network::CustomProtocolObject
	{
		int objectID;
		float position[3];

		Protocol_ObjectRespawn()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectRespawn;
			// ID
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			// POSITION 
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->objectID = 0;
			memset(&this->position[0], 0, sizeof(float) * 3);
		}
		Protocol_ObjectRespawn(int id, float position[3])
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectRespawn;
			// ID
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			// POSITION 
			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->objectID = id;
			memcpy(&this->position[0], &position[0], sizeof(float) * 3);
		}
		Protocol_ObjectRespawn(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt;
			this->position[0] = p[2].value.netFloat;
			this->position[1] = p[3].value.netFloat;
			this->position[2] = p[4].value.netFloat;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			this->protocol[2].value = this->position[0];
			this->protocol[3].value = this->position[1];
			this->protocol[4].value = this->position[2];
			return protocol;		 
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectDie				367
	struct Protocol_ObjectDie :public Oyster::Network::CustomProtocolObject
	{
		int victimID;
		int victimDeathCount;
		int killerID;
		int killerKillCount;
		float seconds;

		Protocol_ObjectDie()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectDie;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			this->victimID = -1;
			this->victimDeathCount = -1;
			this->killerID = -1;
			this->killerKillCount = -1;
			this->seconds = 0.0f;
		}
		Protocol_ObjectDie(int victimID, int deathCount, int killerID, int killCount, float seconds)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectDie;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			this->victimID = victimID;
			this->victimDeathCount = deathCount;
			this->killerID = killerID;
			this->killerKillCount = killCount;
			this->seconds = seconds;
		}
		Protocol_ObjectDie(Oyster::Network::CustomNetProtocol& p)
		{
			this->victimID	= p[1].value.netInt;
			this->victimDeathCount = p[2].value.netInt;
			this->killerID	= p[3].value.netInt;
			this->killerKillCount	= p[4].value.netInt;
			this->seconds	= p[5].value.netFloat;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->victimID;
			this->protocol[2].value = this->victimDeathCount;
			this->protocol[3].value = this->killerID;
			this->protocol[4].value = this->killerKillCount;
			this->protocol[5].value = this->seconds;
			return protocol;		 
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectDisconnectPlayer	368
	struct Protocol_ObjectDisconnectPlayer :public Oyster::Network::CustomProtocolObject
	{
		int objectID;

		Protocol_ObjectDisconnectPlayer()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectDisconnectPlayer;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = 0;
		}
		Protocol_ObjectDisconnectPlayer(int objectID)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Gameplay_ObjectDisconnectPlayer;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->objectID = objectID;
		}
		Protocol_ObjectDisconnectPlayer(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netInt;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->objectID;
			return protocol;		 
		}

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_ObjectAction				369
	struct Protocol_ObjectAction :public Oyster::Network::CustomProtocolObject
	{
		short objectID;
		int animationID; 
		
		Protocol_ObjectAction()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectAction;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;		
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
			
			objectID = -1;
			animationID = -1;
		}
		Protocol_ObjectAction(Oyster::Network::CustomNetProtocol& p)
		{
			objectID = p[1].value.netShort;
			animationID = p[2].value.netInt;
		}
		Protocol_ObjectAction( int id, int animID)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectAction;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;

			objectID = id;
			animationID = animID;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = objectID;
			this->protocol[2].value = animationID;
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
	//#define protocol_Gameplay_ObjectCollision				370
	struct Protocol_ObjectCollision :public Oyster::Network::CustomProtocolObject
	{
		short objectID;
		int collisionID; 
		// TODO: maybe position, impact, and velocity

		Protocol_ObjectCollision()
		{
			this->protocol[0].value = protocol_Gameplay_ObjectCollision;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;		
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;

			this->objectID = -1;
			this->collisionID = -1;
		}
		Protocol_ObjectCollision(Oyster::Network::CustomNetProtocol& p)
		{
			this->objectID = p[1].value.netShort;
			this->collisionID = p[2].value.netInt;
		}
		Protocol_ObjectCollision( int id, int collisionID)
		{
			this->protocol[0].value = protocol_Gameplay_ObjectCollision;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;

			this->objectID = id;
			this->collisionID = collisionID;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = objectID;
			this->protocol[2].value = collisionID;
			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Gameplay_EffectBeam				371
	struct Protocol_EffectBeam : public Oyster::Network::CustomProtocolObject
	{
		short ownerID;
		float startPoint[3], endPoint[3],
			  beamRadius, lifeTime;

		Protocol_EffectBeam()
		{
			this->protocol[0].value = protocol_Gameplay_EffectBeam;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].value = -1;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;

			for( int i = 2; i < 10; ++i )
			{ // seeding startPoint, endPoint, beamRadius & lifeTime value_types
				this->protocol[i].type = Oyster::Network::NetAttributeType_Float;
			}

			this->ownerID		= -1;
			for( int i = 0; i < 3 ; ++i )
			{
				this->startPoint[i]	=
				this->endPoint[i] = 0.0f;
			}
			this->beamRadius	= 0.0f;
			this->lifeTime		= 0.0f;
		}
		Protocol_EffectBeam( Oyster::Network::CustomNetProtocol& p )
		{
			this->ownerID		= p[1].value.netShort;
			for( int i = 0; i < 3 ; ++i )
			{
				this->startPoint[i]	= p[2+i].value.netFloat;
				this->endPoint[i] = p[5+i].value.netFloat;
			}
			this->beamRadius	= p[8].value.netFloat;
			this->lifeTime		= p[9].value.netFloat;
		}
		Protocol_EffectBeam( int creatorID, const float start[3], const float end[3], float radius, float lifeTime )
		{
			this->protocol[0].value = protocol_Gameplay_EffectBeam;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].value = -1;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;

			for( int i = 2; i < 10; ++i )
			{ // seeding startPoint, endPoint, beamRadius & lifeTime value_types
				this->protocol[i].type = Oyster::Network::NetAttributeType_Float;
			}

			this->ownerID		= creatorID;
			for( int i = 0; i < 3 ; ++i )
			{
				this->startPoint[i]	= start[i];
				this->endPoint[i] = end[i];
			}
			this->beamRadius	= radius;
			this->lifeTime		= lifeTime;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = ownerID;

			for( int i = 0; i < 3 ; ++i )
			{
				// startPoint
				this->protocol[2+i].value	= this->startPoint[i];

				// endPoint
				this->protocol[5+i].value	= this->endPoint[i];
			}

			// beamRadius
			this->protocol[8].value = this->beamRadius;

			// lifeTime
			this->protocol[9].value = this->lifeTime;

			return protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
}
#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H