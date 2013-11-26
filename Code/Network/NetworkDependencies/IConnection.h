#ifndef NETWORK_DEPENDENCIES_I_CONNECTION_H
#define NETWORK_DEPENDENCIES_I_CONNECTION_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

namespace Oyster
{
	namespace Network
	{
		class IConnection
		{

		public:
			virtual int Disconnect() = 0;
			virtual int Send( const unsigned char message[] ) = 0;
			virtual int  Recieve(unsigned char message[]) = 0;
			virtual int InitiateServer( unsigned short port ) { return false; };
			virtual int InitiateClient() { return false; };
			virtual int  Listen() { return -1; };
			virtual int Connect( unsigned short port, const char serverName[] ) { return false; };
		};
	}
}

#endif