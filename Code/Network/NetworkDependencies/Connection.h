#ifndef NETWORK_DEPENDENCIES_CONNECTION_H
#define NETWORK_DEPENDENCIES_CONNECTION_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "IConnection.h"
#include "../NetworkDependencies/OysterByte.h"

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

			virtual bool InitiateServer( unsigned short port );
			virtual bool InitiateClient();

			virtual bool Send( OysterByte& bytes );
			virtual int  Recieve( OysterByte& bytes );

			virtual void Disconnect();
			virtual bool Connect( unsigned short port , const char serverName[] );
			virtual int  Listen();

		private:
			bool initiateSocket();

			int socket;

		};
	}
}

#endif