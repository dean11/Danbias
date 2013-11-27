#include "Listener.h"

using namespace Oyster::Network::Server;

Listener::Listener()
{
	newSocket = false;
	tempSocket = 0;
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

	thread.Create(this, true);

	return true;
}

int Listener::GetNewClient()
{
	mutex.LockMutex();
	int temp = -1;

	if(newSocket)
	{
		temp = tempSocket;
		newSocket = false;
	}
	mutex.UnlockMutex();

	return temp;
}

int Listener::Accept()
{
	int clientSocket = 0;
	clientSocket = connection->Listen();

	mutex.LockMutex();
	tempSocket = clientSocket;
	newSocket = true;
	mutex.UnlockMutex();

	return clientSocket;
}

bool Listener::DoWork()
{
	Accept();

	return true;
}

#include <iostream>
void Listener::ThreadEntry()
{
	std::cout << "Thread started" << std::endl;
}

void Listener::ThreadExit()
{
	std::cout << "Thread stopped" << std::endl;
}