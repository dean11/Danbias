#include "Client.h"

using namespace Oyster::Network::Client;

Client::Client()
{
	connection = new Connection();
}

Client::~Client()
{
	delete this->connection;
	connection = 0;
}

bool Client::Connect(unsigned int port, char filename[])
{
	connection->InitiateClient();
	connection->Connect(port, filename);

	return true;
}

void Client::Send(unsigned char msg[])
{
	connection->Send(msg);
}

void Client::Recv(unsigned char msg[])
{
	connection->Recieve(msg);
}