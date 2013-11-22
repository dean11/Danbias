#ifndef NETWORK_CLIENT_CLIENT_H
#define NETWORK_CLIENT_CLIENT_H

/////////////////////////////////////////////////////////////////////
// Created by Pontus Fransson 2013
/////////////////////////////////////////////////////////////////////

#include "../NetworkDependencies/Connection.h"

namespace Oyster
{
	namespace Network
	{
		namespace Client
		{
			class Client
			{
			public:
				Client();
				~Client();

				bool Connect(unsigned int port, char filename[]);

				void Send(unsigned char msg[]);
				void Recv(unsigned char msg[]);

			private:
				::Oyster::Network::Connection* connection;
			};
		}
	}
}

#endif