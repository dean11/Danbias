#ifndef NETWORK_DEPENDENCIES_I_CONNECTION_H
#define NETWORK_DEPENDENCIES_I_CONNECTION_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "Utilities.h"

namespace Oyster
{
	namespace Network
	{
		const int MAX_NETWORK_MESSAGE_SIZE = 1400;

		struct ConnectionInfo
		{
			int socket;
			std::string addr;
		};

		class OysterByte;
		class IConnection
		{

		public:
			
			//sends and recieve functions with bytearrays, 
			//will send to the users connection via socket
			virtual int Send( OysterByte &bytes ) = 0;
			virtual int  Recieve( OysterByte &bytes) = 0;

			//initiates sockets and address for server and client
			virtual int InitiateServer( unsigned short port ) { return false; };
			virtual int InitiateClient() { return false; };

			//Listen function to let client connect, only used by the server
			virtual ConnectionInfo  Listen() { return ConnectionInfo(); };

			//enables the client to connect with a server with use of name and port
			//(servers uses Listen instead of connect)
			virtual int Connect( unsigned short port, const char serverName[] ) { return false; };

			//Tries to connect with the same port and ip used for Connect.
			virtual int Reconnect() = 0;

			//Disconnects the client or server TODO: optimize!
			virtual int Disconnect() = 0;

			virtual int GetSocket() { return -1; };
		};
	}
}

#endif