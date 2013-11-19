#ifndef NETWORK_SERVER_LISTENER_H
#define NETWORK_SERVER_LISTENER_H

/////////////////////////////////////////////////////////////////////
// Created by Pontus 2013
/////////////////////////////////////////////////////////////////////

#include "IListener.h"
#include "../NetworkDependencies/Connection.h"

class Listener
{
public:
	Listener();
	~Listener();

	bool Init(unsigned int port);
	int Accept();

private:
	Connection* connection;

};

#endif