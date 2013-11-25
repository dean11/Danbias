#ifndef NETWORK_SERVER_CLIENT_H
#define NETWORK_SERVER_CLIENT_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "../NetworkDependencies/Connection.h"

namespace Oyster
{
	namespace Network
	{
		namespace Server
		{
			class Client
			{
			public:
				Client(unsigned int socket);
				~Client();

				void Send(unsigned char buffer[]);
				void Recv(unsigned char buffer[]);

			private:
				::Oyster::Network::Connection* connection;

			};
		}
	}
};

#endif