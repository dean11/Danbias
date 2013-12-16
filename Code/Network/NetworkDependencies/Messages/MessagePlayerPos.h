#ifndef NETWORK_DEPENDENCIES_MESSAGE_PLAYER_POS_H
#define NETWORK_DEPENDENCIES_MESSAGE_PLAYER_POS_H

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
			class MessagePlayerPos : public MessageHeader
			{
			public:
				MessagePlayerPos();
				virtual ~MessagePlayerPos();

				virtual void Pack(Protocols::ProtocolHeader& header, OysterByte& bytes);
				virtual void Unpack(OysterByte& bytes, Protocols::ProtocolHeader& header);

			private:



			};
		}
	}
}

#endif