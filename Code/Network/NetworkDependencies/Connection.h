//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef NETWORK_DEPENDENCIES_CONNECTION_H
#define NETWORK_DEPENDENCIES_CONNECTION_H

#include "IConnection.h"

class Connection : public IConnection
{
	private:
		int mySocket;
	
	public:
		Connection();
		Connection(int socket);
		~Connection();

		virtual bool Connect( unsigned short port , const char serverName[] );
		virtual bool InitiateServer( unsigned short port ); 
		virtual void Disconnect();
		virtual bool Send(int socket , const char message[]);
		virtual int  Recieve(int socket);
		virtual int  Listen();
};

#endif