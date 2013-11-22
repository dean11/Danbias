#ifndef NETWORK_DEPENDENCIES_MESSAGE_HEADER_H
#define NETWORK_DEPENDENCIES_MESSAGE_HEADER_H

/////////////////////////////////////////////////////////////////////
// Created by Pontus Fransson 2013
/////////////////////////////////////////////////////////////////////

#include <string>
#include "../Protocols.h"

namespace Oyster
{
	namespace Network
	{
		namespace Messages
		{
			class MessageHeader
			{
			public:
				MessageHeader();
				virtual ~MessageHeader();

				virtual void Translate(Protocols::ProtocolHeader& header, unsigned char msg[] );
				virtual void Translate(unsigned char message[], Protocols::ProtocolHeader& header);


			protected:
				//Add variables to messages
				void AddInt(int i, unsigned char msg[]);
				void AddStr(std::string str, unsigned char msg[]);

				//Get variables from message
				int GetInt(unsigned char message[]);
				std::string GetStr(unsigned char message[]);

			private:
				int size;

			};
		}
	}
}

#endif