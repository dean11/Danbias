#ifndef NETWORK_DEPENDENCIES_I_CONNECTION_H
#define NETWORK_DEPENDENCIES_I_CONNECTION_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

namespace Oyster
{
	namespace Network
	{
		class OysterByte;
		class IConnection
		{

		public:
			virtual int Disconnect() = 0;
			virtual int Send( OysterByte& bytes ) = 0;
			virtual int  Recieve( OysterByte& bytes) = 0;
			virtual int InitiateServer( unsigned short port ) { return false; };
			virtual int InitiateClient() { return false; };
			virtual int  Listen() { return -1; };
			virtual int Connect( unsigned short port, const char serverName[] ) { return false; };
		};
	}
}

#endif