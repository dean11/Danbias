#ifndef GAMELOGIC_GENERAL_PROTOCOLS_H
#define GAMELOGIC_GENERAL_PROTOCOLS_H

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
			States_disconected,
			States_leave
		};
		States status;
	
		Protocol_General_Status()
		{
			this->protocol[0].value = protocol_General_Status;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_General_Status(States state)
		{
			this->protocol[0].value = protocol_General_Status;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			this->status = state;
		}
	
		Protocol_General_Status(Oyster::Network::CustomNetProtocol& p)
		{
			status = (States)this->protocol[1].value.netShort;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = status;

			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_General_Text :public Oyster::Network::CustomProtocolObject
	{
		std::string text;	//The text to send
		int destination;	//The destination if any (Ie a whisper to a player)

		Protocol_General_Text()
			: destination(-1) {}
		Protocol_General_Text(Oyster::Network::CustomNetProtocol& p)
		{
			destination = p.Get(1).value.netInt;
			text = p.Get(2).value.netCharPtr;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol.Set(0, protocol_General_Text, Oyster::Network::NetAttributeType_Short);
			this->protocol.Set(1, destination, Oyster::Network::NetAttributeType_Int);
			this->protocol.Set(2, text);
			return protocol;
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Broadcast_Test							102
	struct Protocol_Broadcast_Test :public Oyster::Network::CustomProtocolObject
	{
		int test;

		Protocol_Broadcast_Test()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Broadcast_Test;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->test = 0;
		}
		Protocol_Broadcast_Test(int test)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Broadcast_Test;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->test = test;
		}
		Protocol_Broadcast_Test(Oyster::Network::CustomNetProtocol& p)
		{
			this->test = p[1].value.netInt;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->test;
			
			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
}
#endif //!GAMELOGIC_CONTROL_PROTOCOLS_H