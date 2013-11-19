//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef NETWORK_DEPENDENCIES_I_CONNECTION_H
#define NETWORK_DEPENDENCIES_I_CONNECTION_H

namespace Oyster
{
	namespace Network
	{
		class IConnection
		{
			public:
				virtual void Disconnect() = 0;
				virtual bool Send( const char message[] ) = 0;
				virtual int  Recieve(char message[]) = 0;
				virtual bool InitiateServer( unsigned short port ) { return false; };
				virtual int  Listen() { return -1; };
				virtual bool Connect( unsigned short port, const char serverName[] ) { return false; };
		};
	}
}

#endif