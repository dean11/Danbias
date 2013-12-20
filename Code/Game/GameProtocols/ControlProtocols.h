#ifndef GAMELOGIC_CONTROL_PROTOCOLS_H
#define GAMELOGIC_CONTROL_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"

namespace GameLogic
{
	struct Protocol_General_Status :public Oyster::Network::CustomProtocolObject
	{
		enum States
		{
			States_ready,
			States_idle,
			States_bussy,
			States_disconected,
		};
		States status;
	
		Protocol_General_Status()
		{
			this->protocol[0].value = protocol_General_Status;
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
	
	struct Protocol_General_Ping :public Oyster::Network::CustomProtocolObject
	{

		Protocol_General_Ping()
		{
			this->protocol[0].value = protocol_General_Ping;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			return &protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_General_Disconnect :public Oyster::Network::CustomProtocolObject
	{
		Protocol_General_Disconnect()
		{
			this->protocol[0].value = protocol_General_Disconnect;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			return &protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_General_DisconnectKick :public Oyster::Network::CustomProtocolObject
	{
		Protocol_General_DisconnectKick()
		{
			this->protocol[0].value = protocol_General_DisconnectKick;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			return &protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_General_Text :public Oyster::Network::CustomProtocolObject
	{
		char* text;

		Protocol_General_Text()
		{
			this->protocol[0].value = protocol_General_Text;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_CharArray;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value.netCharPtr = text;
			return &protocol;
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

}
#endif //!GAMELOGIC_CONTROL_PROTOCOLS_H