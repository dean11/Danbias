#include "Client.h"

using namespace Oyster::Network::Client;

Client::Client()
{
	connection = new Connection();
}

Client::~Client()
{
	delete connection;
}

bool Client::Connect(unsigned int port, char filename[])
{
	connection->Connect(port, filename);

	return true;
}

void Client::Send(char msg[])
{
	connection->Send(msg);
}

void Client::Recv(char msg[])
{
	connection->Recieve(msg);
}