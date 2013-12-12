#ifndef GAMESERVER_TEST_H
#define GAMESERVER_TEST_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"



namespace GameLogic
{
	struct Protocol_TEST :public Oyster::Network::CustomProtocolObject
	{
		int ProtocolID;
		char text;
		
		Protocol_TEST()
		{
			this->protocol[0].value = ProtocolID		= PROTOCOL_TEST;

			this->protocol[0].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Char;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value = text;

			return &protocol;
		}

		private:
			Oyster::Network::CustomNetProtocol protocol;
	};
}

#endif // !GAMESERVER_TEST_H