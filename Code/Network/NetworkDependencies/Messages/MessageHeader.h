#ifndef NETWORK_DEPENDENCIES_MESSAGE_HEADER_H
#define NETWORK_DEPENDENCIES_MESSAGE_HEADER_H

/////////////////////////////////////////////////////////////////////
// Created by Pontus Fransson 2013
/////////////////////////////////////////////////////////////////////

#include <string>

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

				virtual void Translate(/*Message& msg*/);
				virtual void Translate(unsigned char message[]);

				unsigned char* GetMsg();

			protected:
				//Add variables to messages
				void AddInt(int i);
				void AddStr(std::string str);

				//Get variables from message
				int GetInt(unsigned char message[]);
				std::string GetStr(unsigned char message[]);

			private:
				static const int max_message_length = 1024;
				unsigned char* msg;
				int size;

			};
		}
	}
}

#endif