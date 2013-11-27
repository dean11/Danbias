#include "Client.h"

using namespace Oyster::Network;
using namespace Oyster::Network::Server;

Client::Client(unsigned int socket)
{
	connection = new Connection(socket);
}

Client::~Client()
{
	delete connection;
}

void Client::Send(OysterByte& bytes)
{
	connection->Send(bytes);
}

void Client::Recv(OysterByte& bytes)
{
	connection->Recieve(bytes);
}