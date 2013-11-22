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

void Client::Send(unsigned char buffer[])
{
	connection->Send(buffer);
}

void Client::Recv(unsigned char buffer[])
{
	connection->Recieve(buffer);
}