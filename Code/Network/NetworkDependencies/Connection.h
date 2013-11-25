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

			virtual bool Connect( unsigned short port , const char serverName[] );
			virtual bool InitiateServer( unsigned short port );
			//virutal bool initiateClient();

			virtual void Disconnect();

			virtual bool Send(const unsigned char message[]);
			virtual int  Recieve(unsigned char message[]);

			virtual int  Listen();

		private:
			int socket;

		};
	}
}

#endif