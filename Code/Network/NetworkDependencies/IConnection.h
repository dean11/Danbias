//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef NETWORK_DEPENDENCIES_I_CONNECTION_H
#define NETWORK_DEPENDENCIES_I_CONNECTION_H

#include <winsock2.h>
#include <iostream>

class IConnection
{
	public:
		virtual bool Connect( unsigned short port, const char serverName[] ) = 0;
		virtual bool InitiateServer( unsigned short port ) = 0; 
		virtual void Disconnect() = 0;
		virtual bool Send(int socket , const char message[]) = 0;
		virtual int  Recieve( int socket ) = 0;
		virtual int  Listen() = 0;
};

#endif