#include "Listener.h"

using namespace Oyster::Network::Server;
using namespace Utility::DynamicMemory;
using namespace Oyster::Thread;

Listener::Listener()
{
	connection = NULL;
}

Listener::Listener(Oyster::Network::IPostBox<int>* postBox)
{
	connection = NULL;
	this->postBox = postBox;
}

Listener::~Listener()
{
	Stop();
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

	if(thread.Create(this, true) == OYSTER_THREAD_ERROR_FAILED)
	{
		return false;
	}

	return true;
}

bool Listener::Init(unsigned int port, bool start)
{
	connection = new Connection();
	if(connection->InitiateServer(port) != 0)
	{
		return false;
	}

	if(thread.Create(this, start) == OYSTER_THREAD_ERROR_FAILED)
	{
		return false;
	}

	return true;
}

bool Listener::Start()
{
	if(thread.Start() == OYSTER_THREAD_ERROR_FAILED)
	{
		return false;
	}

	return true;
}

void Listener::Stop()
{
	thread.Stop();
}

void Listener::Shutdown()
{
	thread.Stop();
}

void Listener::SetPostBox(Oyster::Network::IPostBox<int>* postBox)
{
	stdMutex.lock();
	this->postBox = postBox;
	stdMutex.unlock();
}

int Listener::Accept()
{
	int clientSocket = -1;
	clientSocket = connection->Listen();

	if(clientSocket != -1)
	{
		stdMutex.lock();
		postBox->PostMessage(clientSocket);
		stdMutex.unlock();
	}

	return clientSocket;
}

bool Listener::DoWork()
{
	int result = Accept();

	if(result == -1)
	{
		//Do something?
	}

	return true;
}

void Listener::ThreadEntry()
{
}

void Listener::ThreadExit()
{
}