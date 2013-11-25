#ifndef NETWORK_DEPENDENCIES_MESSAGE_TEST_H
#define NETWORK_DEPENDENCIES_MESSAGE_TEST_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "MessageHeader.h"

namespace Oyster
{
	namespace Network
	{
		namespace Messages
		{
			class MessageTest : public MessageHeader
			{
			public:
				MessageTest();
				virtual ~MessageTest();

				virtual void Pack(Protocols::ProtocolHeader& header, unsigned char msg[]);
				virtual void Unpack(unsigned char msg[], Protocols::ProtocolHeader& header);

			private:
			};
		}
	}
}

#endif