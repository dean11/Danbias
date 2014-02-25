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

			virtual int Send( OysterByte &bytes );
			virtual int Recieve( OysterByte &bytes );

			virtual int Disconnect();
			virtual int Connect(ConnectionInfo info, bool blocking = false);
			virtual int Connect(unsigned short port , const char serverName[], bool blocking = false);
			virtual int Reconnect();
			
			virtual ConnectionInfo Listen();

			bool IsSending();
			bool IsConnected();

			//Setting the socket to blocking/non-blocking mode.
			int SetBlockingMode( bool blocking );

			std::string GetIpAddress();
			int GetSocket();


		private:
			int InitiateSocket();

			int socket;

			bool stillSending;
			bool closed;
			std::string addr;

			std::string lastConnectAddr;
			unsigned short lastConnectPort;

			bool blocking;
		};
	}
}

#endif