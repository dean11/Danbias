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

void Client::Send(Oyster::Network::OysterByte& bytes)
{
	connection->Send(bytes);
}

void Client::Recv(Oyster::Network::OysterByte& bytes)
{
	connection->Recieve(bytes);
}