#ifndef GAMELOGIC_OBJECT_PROTOCOLS_H
#define GAMELOGIC_OBJECT_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"



namespace GameLogic
{
	struct Protocol_CreateObject :public Oyster::Network::CustomProtocolObject
	{
		int object_ID;
		char *path;
		float worldMatrix[16];


		Protocol_CreateObject()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_CreateObject;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_CharArray; 
			
			for (int i = 3; i <= 18; i++)
			{
				this->protocol[i].type = Oyster::Network::NetAttributeType_Float;
			}
		}
		Protocol_CreateObject(float m[16], int id, char *path)
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_CreateObject;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Int;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_CharArray; 
			
			for (int i = 3; i <= 18; i++)
			{
				this->protocol[i].type = Oyster::Network::NetAttributeType_Float;
			}

			object_ID = id;
			this->path = path;
			memcpy(&worldMatrix[0], &m[0], sizeof(float)*16);
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{

			this->protocol[1].value = object_ID;
			this->protocol[2].value = path;
			this->protocol[3].value = worldMatrix[0];
			this->protocol[4].value = worldMatrix[1]; 
			this->protocol[5].value	= worldMatrix[2];
			this->protocol[6].value	= worldMatrix[3];
			this->protocol[7].value	= worldMatrix[4];
			this->protocol[8].value = worldMatrix[5];
			this->protocol[9].value = worldMatrix[6];
			this->protocol[10].value = worldMatrix[7];
			this->protocol[11].value = worldMatrix[8];
			this->protocol[12].value = worldMatrix[9];
			this->protocol[13].value = worldMatrix[10];
			this->protocol[14].value = worldMatrix[11];
			this->protocol[15].value = worldMatrix[12];
			this->protocol[16].value = worldMatrix[13];
			this->protocol[17].value = worldMatrix[14];
			this->protocol[18].value = worldMatrix[15];

		
		

			return &protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
	
	struct Protocol_ObjectPosition :public Oyster::Network::CustomProtocolObject
	{
		int object_ID;
		float worldMatrix[16];
		// look at dir 

		Protocol_ObjectPosition()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_ObjectPosition;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Int;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			
			for (int i = 2; i <= 17; i++)
			{
				this->protocol[i].type = Oyster::Network::NetAttributeType_Float;
			}
			object_ID = -1;
			memset(&worldMatrix[0], 0, sizeof(float) * 16);
		}
		Protocol_ObjectPosition(float m[16], int id)
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_ObjectPosition;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Int;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			
			for (int i = 2; i <= 17; i++)
			{
				this->protocol[i].type = Oyster::Network::NetAttributeType_Float;
			}

			object_ID = id;
			memcpy(&worldMatrix[0], &m[0], sizeof(float)*16);
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value = object_ID;
			for (int i = 2; i <= 17; i++)
			{
				this->protocol[i].value = worldMatrix[i-2];
			}
			return &protocol;		 
		}	

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_RemoveObject :public Oyster::Network::CustomProtocolObject
	{
		int object_ID;

		Protocol_RemoveObject()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_RemoveObject;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{

			this->protocol[1].value = object_ID;
			return &protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H