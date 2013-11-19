#include "Client.h"

Client::Client(unsigned int socket)
{
	connection = new Connection(socket);
}

Client::~Client()
{
	delete connection;
}

void Client::Send(char buffer[])
{
	connection->Send(buffer);
}