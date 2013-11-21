#ifndef NETWORK_SERVER_CLIENT_H
#define NETWORK_SERVER_CLIENT_H

/////////////////////////////////////////////////////////////////////
// Created by Pontus Fransson 2013
/////////////////////////////////////////////////////////////////////

#include "../NetworkDependencies/Connection.h"

class Client
{
public:
	Client(unsigned int socket);
	~Client();

	void Send(char buffer[]);
	void Recv(char buffer[]);

private:
	Connection* connection;

};

#endif