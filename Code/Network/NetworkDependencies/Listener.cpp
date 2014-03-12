#include "Listener.h"

using namespace Utility::DynamicMemory;
using namespace Oyster::Thread;
using namespace Oyster::Network;

Listener::Listener()
{
	this->port = -1;
	this->isListening = false;
	connection = NULL;
}

Listener::Listener(Oyster::Network::IPostBox<ConnectionInfo>* postBox)
{
	this->isListening = false;
	connection = NULL;
	this->postBox = postBox;
}

Listener::~Listener()
{
	if(connection)
	{
		this->isListening = false;
		this->thread.Terminate();
		delete connection;
		connection = 0;
		this->port = -1;
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

	this->port = port;
	this->isListening = true;
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

	if(start)
	{
		this->isListening = true;
	}
	this->port = port;
	return true;
}

bool Listener::Start()
{
	if(thread.Start() == OYSTER_THREAD_ERROR_FAILED)
	{
		return false;
	}

	this->isListening = true;
	return true;
}

void Listener::Stop()
{
	StopListen();
}

void Listener::Shutdown()
{
	StopListen();
}

void Listener::SetPostBox(Oyster::Network::IPostBox<ConnectionInfo>* postBox)
{
	stdMutex.lock();
	this->postBox = postBox;
	stdMutex.unlock();
}

int Listener::Accept()
{
	ConnectionInfo clientSocket = {0};
	clientSocket = connection->Listen();

	if(!this->isListening.load())
	{
		return -1;
	}
	if(clientSocket.socket != -1)
	{
		stdMutex.lock();
		postBox->PostMessage(clientSocket);
		stdMutex.unlock();
	}

	return clientSocket.socket;
}
void Listener::StopListen()
{
	if(this->connection && this->connection->IsConnected())
	{
		this->isListening = false;
		Connection c;
		c.InitiateClient();
		c.Connect(this->port, "127.0.0.1", false);
	}
}
bool Listener::DoWork()
{
	if(!this->connection) return false;
	int result = Accept();

	if(!this->isListening.load())
	{
		return false;
	}
	else if(result == -1)
	{
		//Do something?
	}

	return true;
}

void Listener::ThreadEntry()
{
}

Oyster::Thread::IThreadObject::ThreadCode Listener::ThreadExit()
{
	return Thread::IThreadObject::ThreadCode_Exit;
}