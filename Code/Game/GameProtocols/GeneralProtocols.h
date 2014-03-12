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
			States_leave,
			State_serverShutdown,
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
			status = (States)p[1].value.netShort;
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

	//#define protocol_General_GameOver							10
	struct Protocol_General_GameOver :public Oyster::Network::CustomProtocolObject
	{
		float resetTime;

		Protocol_General_GameOver()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_General_GameOver;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
		}
		Protocol_General_GameOver(float resetTime)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_General_GameOver;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->resetTime = resetTime;
		}
		Protocol_General_GameOver(Oyster::Network::CustomNetProtocol& p)
		{
			this->resetTime = p[1].value.netFloat;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->resetTime;
			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	//#define protocol_Broadcast_Test							103
	struct Protocol_Broadcast_Test :public Oyster::Network::CustomProtocolObject
	{
		unsigned short port;
		std::string ip;
		std::string name;

		Protocol_Broadcast_Test()
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Broadcast_Test;
			this->protocol[1].type = Oyster::Network::NetAttributeType_UnsignedShort;
			this->protocol[2].type = Oyster::Network::NetAttributeType_CharArray;
			this->protocol[3].type = Oyster::Network::NetAttributeType_CharArray;
			this->port = 0;
		}
		Protocol_Broadcast_Test(unsigned short port, std::string ip, std::string name)
		{ 
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[0].value.netShort = protocol_Broadcast_Test;
			this->protocol[1].type = Oyster::Network::NetAttributeType_UnsignedShort;
			this->port = port;
			this->protocol[2].type = Oyster::Network::NetAttributeType_CharArray;
			this->ip = ip;
			this->protocol[3].type = Oyster::Network::NetAttributeType_CharArray;
			this->name = name;
		}
		Protocol_Broadcast_Test(Oyster::Network::CustomNetProtocol& p)
		{
			this->port = p[1].value.netUShort;
			this->ip.assign(p[2].value.netCharPtr);
			this->name.assign(p[3].value.netCharPtr);
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->port;
			this->protocol.Set(2, ip);
			this->protocol.Set(3, name);
			
			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_General_Timer :public Oyster::Network::CustomProtocolObject
	{
		float timeLeft;
	
		Protocol_General_Timer()
		{
			this->protocol[0].value = protocol_General_Timer;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
		}
		Protocol_General_Timer(float time)
		{
			this->protocol[0].value = protocol_General_Timer;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			this->timeLeft = time;
		}
	
		Protocol_General_Timer(Oyster::Network::CustomNetProtocol& p)
		{
			this->timeLeft = p[1].value.netFloat;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = this->timeLeft;

			return protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
}
#endif //!GAMELOGIC_CONTROL_PROTOCOLS_H