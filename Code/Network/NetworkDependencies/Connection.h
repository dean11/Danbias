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
			Connection();
			Connection( int socket );
			virtual ~Connection();

			virtual int InitiateServer( unsigned short port );
			virtual int InitiateClient();

			virtual int Send( Utility::DynamicMemory::SmartPointer<OysterByte> &bytes );
			virtual int  Recieve( Utility::DynamicMemory::SmartPointer<OysterByte> &bytes );

			virtual int Disconnect();
			virtual int Connect( unsigned short port , const char serverName[] );
			
			virtual int  Listen();

			//Setting the socket to blocking/non-blocking mode.
			int SetBlockingMode( bool blocking );

		private:
			int InitiateSocket();

			int socket;

			bool stillSending;
			bool closed;

		};
	}
}

#endif