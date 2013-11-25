#ifndef NETWORK_DEPENDENCIES_MESSAGE_HEADER_H
#define NETWORK_DEPENDENCIES_MESSAGE_HEADER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

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

				virtual void Pack(Protocols::ProtocolHeader& header, unsigned char msg[] );
				virtual void Unpack(unsigned char msg[], Protocols::ProtocolHeader& header);

			protected:
				//Pack variables to messages
				void PackBool(bool i, unsigned char msg[]);

				void PackChar(char i, unsigned char msg[]);
				void PackUnsignedChar(unsigned char i, unsigned char msg[]);

				void PackShort(short i, unsigned char msg[]);
				void PackUnsignedShort(unsigned short i, unsigned char msg[]);

				void PackInt(int i, unsigned char msg[]);
				void PackUnsignedInt(unsigned int i, unsigned char msg[]);

				void PackInt64(__int64 i, unsigned char msg[]);
				void PackUnsignedInt64(unsigned __int64 i, unsigned char msg[]);

				void PackFloat(float i, unsigned char msg[]);
				void PackFloat(float i[], unsigned int elementCount, unsigned char msg[]);
				void PackDouble(double i, unsigned char msg[]);

				void PackStr(char str[], unsigned char msg[]);
				void PackStr(std::string str, unsigned char msg[]);

				//TODO: Add Pack functions for Vec2, 3, 4 and maybe Matrix. Etc.


				//Unpack variables from message
				bool UnpackBool(unsigned char msg[]);

				char UnpackChar(unsigned char msg[]);
				unsigned char UnpackUnsignedChar(unsigned char msg[]);

				short UnpackShort(unsigned char msg[]);
				unsigned short UnpackUnsignedShort(unsigned char msg[]);

				int UnpackInt(unsigned char msg[]);
				unsigned int UnpackUnsignedInt(unsigned char msg[]);

				__int64 UnpackInt64(unsigned char msg[]);
				unsigned __int64 UnpackUnsignedInt64(unsigned char msg[]);

				float UnpackFloat(unsigned char msg[]);
				float* UnpackFloat(unsigned int& elementCount, unsigned char msg[]);
				double UnpackDouble(unsigned char msg[]);

				std::string UnpackStr(unsigned char msg[]);

				//TODO: Add Unpack functions for Vec2, 3, 4 and maybe Matrix. Etc.


				//Sets the this->size to first position in msg
				void SetSize(unsigned char msg[]);

			private:
				int size;

			};
		}
	}
}

#endif