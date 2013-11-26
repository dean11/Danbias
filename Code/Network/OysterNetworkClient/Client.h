#ifndef NETWORK_CLIENT_CLIENT_H
#define NETWORK_CLIENT_CLIENT_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

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

				int Connect(unsigned int port, char filename[]);

				int Send(unsigned char msg[]);
				int Recv(unsigned char msg[]);

			private:
				::Oyster::Network::Connection* connection;
			};
		}
	}
}

#endif