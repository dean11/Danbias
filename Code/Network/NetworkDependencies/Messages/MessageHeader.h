#ifndef NETWORK_DEPENDENCIES_MESSAGE_HEADER_H
#define NETWORK_DEPENDENCIES_MESSAGE_HEADER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include <string>
#include "../Protocols.h"
#include "../OysterByte.h"

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

				virtual void Pack(Protocols::ProtocolHeader& header, OysterByte& bytes );
				virtual void Unpack(OysterByte& bytes, Protocols::ProtocolHeader& header);

				//Pack variables to messages
				void PackBool(bool i, OysterByte& bytes);

				void PackChar(char i, OysterByte& bytes);
				void PackUnsignedChar(unsigned char i, OysterByte& bytes);

				void PackShort(short i, OysterByte& bytes);
				void PackUnsignedShort(unsigned short i, OysterByte& bytes);

				void PackInt(int i, OysterByte& bytes);
				void PackUnsignedInt(unsigned int i, OysterByte& bytes);

				void PackInt64(__int64 i, OysterByte& bytes);
				void PackUnsignedInt64(unsigned __int64 i, OysterByte& bytes);

				void PackFloat(float i, OysterByte& bytes);
				void PackFloat(float i[], unsigned int elementCount, OysterByte& bytes);
				void PackDouble(double i, OysterByte& bytes);

				void PackStr(char str[], OysterByte& bytes);
				void PackStr(std::string str, OysterByte& bytes);

				//Unpack variables from message
				bool UnpackBool(OysterByte& bytes);

				char UnpackChar(OysterByte& bytes);
				unsigned char UnpackUnsignedChar(OysterByte& bytes);

				short UnpackShort(OysterByte& bytes);
				unsigned short UnpackUnsignedShort(OysterByte& bytes);

				int UnpackInt(OysterByte& bytes);
				unsigned int UnpackUnsignedInt(OysterByte& bytes);

				__int64 UnpackInt64(OysterByte& bytes);
				unsigned __int64 UnpackUnsignedInt64(OysterByte& bytes);

				float UnpackFloat(OysterByte& bytes);
				float* UnpackFloat(unsigned int& elementCount, OysterByte& bytes);
				double UnpackDouble(OysterByte& bytes);

				char* UnpackCStr(OysterByte& bytes);
				std::string UnpackStr(OysterByte& bytes);

				//Sets the this->size to the first position in msg
				void SetSize(OysterByte& bytes);

				void SetSize(unsigned int size);

			private:
				unsigned int size;

			};
		}
	}
}

#endif