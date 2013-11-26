#include "Client.h"

using namespace Oyster::Network::Server;

Client::Client(unsigned int socket)
{
	connection = new Connection(socket);
}

Client::~Client()
{
	delete connection;
}

int Client::Send(unsigned char buffer[])
{
	return connection->Send(buffer);
}

int Client::Recv(unsigned char buffer[])
{
	return connection->Recieve(buffer);
}