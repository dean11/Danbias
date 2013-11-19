#include "Listener.h"

Listener::Listener()
{

}

Listener::~Listener()
{
	if(connection)
	{
		delete connection;
	}
}

bool Listener::Init(unsigned int port)
{
	connection = new Connection();

	connection->InitiateServer(port);


	return true;
}

int Listener::Accept()
{
	int clientSocket = 0;
	clientSocket = connection->Listen();

	return clientSocket;
}