#include "Listener.h"

using namespace Oyster::Network::Server;
using namespace Utility::DynamicMemory;

Listener::Listener()
{
	connection = NULL;
}

Listener::Listener(Oyster::Network::IPostBox<SmartPointer<int>>* postBox)
{
	connection = NULL;
	this->postBox = postBox;
}

Listener::~Listener()
{
	if(connection)
	{
		delete connection;
	}
}

//Starts the thread immediate
bool Listener::Init(unsigned int port)
{
	connection = new Connection();
	connection->InitiateServer(port);

	thread.Create(this, true);

	return true;
}

bool Listener::Init(unsigned int port, bool start)
{
	connection = new Connection();
	connection->InitiateServer(port);

	thread.Create(this, start);

	return true;
}

void Listener::Start()
{
	thread.Start();
}

void Listener::Stop()
{
	thread.Stop();
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

	if(*clientSocket != -1)
	{
		mutex.LockMutex();
		postBox->PostMessage(clientSocket);
		mutex.UnlockMutex();
	}

	return clientSocket;
}

bool Listener::DoWork()
{
	Accept();

	return true;
}

void Listener::ThreadEntry()
{
}

void Listener::ThreadExit()
{
}