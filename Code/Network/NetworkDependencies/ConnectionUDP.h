#ifndef NETWORK_DEPENDENCIES_CONNECTIONUDP_H
#define NETWORK_DEPENDENCIES_CONNECTIONUDP_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "IConnection.h"
#include "../NetworkDependencies/OysterByte.h"

namespace Oyster
{
	namespace Network
	{
		class ConnectionUDP : public IConnection
		{
		public:
			ConnectionUDP();
			ConnectionUDP( int socket);
			virtual ~ConnectionUDP();

			virtual int InitiateServer( unsigned short port );
			virtual int InitiateClient();

			virtual int InitiateBroadcastServer(unsigned short port, const char serverName[]);
			virtual int InitiateBroadcastClient(unsigned short port);

			virtual int Send ( OysterByte &byte );
			virtual int Recieve( OysterByte &byte );

			virtual int Connect( unsigned short port, const char serverName[] );

			//Doesn't do anything now.
			virtual int Reconnect() { return 0; }

			virtual int Disconnect();

			bool IsSending();
			bool IsConnected();
			int GetIpSize();
			int GetSocket();

			int SetBlockingMode( bool blocking );

		private:
			int InitiateSocket();

			int socket;
			//char ipAddress[16];
			//unsigned short port;
			int ipSize;
			
			unsigned long Address;
			unsigned short port;

			bool stillSending;
			bool closed;
		};
	}
}

#endif
