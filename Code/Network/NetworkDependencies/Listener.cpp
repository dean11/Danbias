#include "Listener.h"

using namespace Oyster::Network::Server;
using namespace Utility::DynamicMemory;

Listener::Listener()
{
	connection = NULL;
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

void Listener::Shutdown()
{
	thread.Stop();
}

void Listener::SetPostBox(Oyster::Network::IPostBox<SmartPointer<int>>* postBox)
{
	mutex.LockMutex();
	this->postBox = postBox;
	mutex.UnlockMutex();
}

int Listener::Accept()
{
	SmartPointer<int> clientSocket = SmartPointer<int>(new int());
	*clientSocket = connection->Listen();

	mutex.LockMutex();
	postBox->PostMessage(clientSocket);
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