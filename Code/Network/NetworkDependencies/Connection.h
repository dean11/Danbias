#ifndef NETWORK_DEPENDENCIES_CONNECTION_H
#define NETWORK_DEPENDENCIES_CONNECTION_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "IConnection.h"

namespace Oyster
{
	namespace Network
	{
		class Connection : public IConnection
		{

		public:
			Connection()           { this->socket = 0; };
			Connection(int socket) { this->socket = socket; };
			virtual ~Connection();

			virtual int InitiateServer( unsigned short port );
			virtual int InitiateClient();

			virtual int Send(const unsigned char message[]);
			virtual int  Recieve(unsigned char message[]);

			virtual int Disconnect();
			virtual int Connect( unsigned short port , const char serverName[] );
			virtual int  Listen();

		private:
			int initiateSocket();

			int socket;

		};
	}
}

#endif