#ifndef GAMELOGIC_PLAYER_PROTOCOLS_H
#define GAMELOGIC_PLAYER_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"



namespace GameLogic
{
	struct Protocol_CreateObject :public Oyster::Network::CustomProtocolObject
	{
		int object_ID;
		char path[255];
		float worldMatrix[16];


		Protocol_CreateObject()
		{
			this->protocol[0].value = protocol_Gamplay_CreateObject;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Int;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_CharArray; 
			
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[7].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[8].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[9].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[10].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[11].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[12].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[13].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[14].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[15].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[16].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[17].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[18].type = Oyster::Network::NetAttributeType_Float;

		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{

			this->protocol[1].value = object_ID;
			this->protocol[2].value = path;
			this->protocol[3].value = worldMatrix[1];
			this->protocol[4].value = worldMatrix[2]; 
			this->protocol[5].value	= worldMatrix[3];
			this->protocol[6].value	= worldMatrix[4];
			this->protocol[7].value	= worldMatrix[5];
			this->protocol[8].value = worldMatrix[6];
			this->protocol[9].value = worldMatrix[7];
			this->protocol[10].value = worldMatrix[8];
			this->protocol[11].value = worldMatrix[9];
			this->protocol[12].value = worldMatrix[10];
			this->protocol[13].value = worldMatrix[11];
			this->protocol[14].value = worldMatrix[12];
			this->protocol[15].value = worldMatrix[13];
			this->protocol[16].value = worldMatrix[14];
			this->protocol[17].value = worldMatrix[15];
			this->protocol[18].value = worldMatrix[16];

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
			this->protocol[0].value = protocol_Gamplay_ObjectPosition;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Int;
									
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;

			this->protocol[2].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[3].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[4].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[5].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[6].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[7].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[8].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[9].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[10].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[11].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[12].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[13].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[14].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[15].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[16].type = Oyster::Network::NetAttributeType_Float;
			this->protocol[17].type = Oyster::Network::NetAttributeType_Float;

		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{

			this->protocol[1].value = object_ID;
			this->protocol[2].value = worldMatrix[1];
			this->protocol[3].value = worldMatrix[2]; 
			this->protocol[4].value	= worldMatrix[3];
			this->protocol[5].value	= worldMatrix[4];
			this->protocol[6].value	= worldMatrix[5];
			this->protocol[7].value = worldMatrix[6];
			this->protocol[8].value = worldMatrix[7];
			this->protocol[9].value = worldMatrix[8];
			this->protocol[10].value = worldMatrix[9];
			this->protocol[11].value = worldMatrix[10];
			this->protocol[12].value = worldMatrix[11];
			this->protocol[13].value = worldMatrix[12];
			this->protocol[14].value = worldMatrix[13];
			this->protocol[15].value = worldMatrix[14];
			this->protocol[16].value = worldMatrix[15];
			this->protocol[17].value = worldMatrix[16];
			return &protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H