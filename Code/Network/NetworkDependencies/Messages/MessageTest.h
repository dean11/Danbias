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

				virtual void Pack(Protocols::ProtocolHeader& header, OysterByte& bytes);
				virtual void Unpack(OysterByte& bytes, Protocols::ProtocolHeader& header);

			private:
			};
		}
	}
}

#endif