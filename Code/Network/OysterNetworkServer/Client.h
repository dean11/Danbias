#ifndef NETWORK_SERVER_CLIENT_H
#define NETWORK_SERVER_CLIENT_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "../NetworkDependencies/Connection.h"
#include "../NetworkDependencies/OysterByte.h"

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

				void Send(OysterByte& bytes);
				void Recv(OysterByte& bytes);

			private:
				::Oyster::Network::Connection* connection;

			};
		}
	}
};

#endif