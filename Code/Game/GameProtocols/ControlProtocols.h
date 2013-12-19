#ifndef GAMELOGIC_CONTROL_PROTOCOLS_H
#define GAMELOGIC_CONTROL_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"

namespace GameLogic
{
	struct Protocol_Status :public Oyster::Network::CustomProtocolObject
	{
		enum States
		{
			States_ready,
			States_idle,
			States_bussy,
			States_disconected,
		};
		States status;
	
		Protocol_Status()
		{
			this->protocol[0].value = protocol_Status;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value = status;

			return &protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
}
#endif //!GAMELOGIC_CONTROL_PROTOCOLS_H