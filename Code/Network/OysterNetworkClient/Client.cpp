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

int Client::Connect(unsigned int port, char filename[])
{
	int errorCode;

	if((errorCode = connection->InitiateClient()) != 0)
	{
		return errorCode;
	}

	if((errorCode = connection->Connect(port, filename)) != 0)
	{
		return errorCode;
	}

	return 0;
}

int Client::Send(unsigned char msg[])
{
	return connection->Send(msg);
}

int Client::Recv(unsigned char msg[])
{
	return connection->Recieve(msg);
}