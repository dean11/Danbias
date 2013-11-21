#ifndef NETWORK_CLIENT_CLIENT_H
#define NETWORK_CLIENT_CLIENT_H

/////////////////////////////////////////////////////////////////////
// Created by Pontus Fransson 2013
/////////////////////////////////////////////////////////////////////

#include "../NetworkDependencies/Connection.h"

class Client
{
public:
	Client();
	~Client();

	bool Connect(unsigned int port, char filename[]);

	void Send(char msg[]);
	void Recv(char msg[]);

private:
	Connection* connection;

};

#endif