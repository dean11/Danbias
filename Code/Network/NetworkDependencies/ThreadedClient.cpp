#include "ThreadedClient.h"
#include "OysterByte.h"

#include <iostream>
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace Utility::Container;
using namespace Utility::DynamicMemory;

ThreadedClient::ThreadedClient()
{
	this->connection = new Connection();
	this->sendPostBox = new PostBox<CustomNetProtocol*>;
	this->recvPostBox = NULL;

	connection->SetBlockingMode(false);
}

ThreadedClient::ThreadedClient(unsigned int socket)
{
	this->connection = new Connection(socket);
	this->sendPostBox = new PostBox<CustomNetProtocol*>;
	this->recvPostBox = NULL;

	connection->SetBlockingMode(false);

	thread.Create(this, true);
}

ThreadedClient::ThreadedClient(IPostBox<CustomNetProtocol*>* postBox, unsigned int socket)
{
	this->connection = new Connection(socket);
	this->sendPostBox = new PostBox<CustomNetProtocol*>;
	this->recvPostBox = postBox;

	connection->SetBlockingMode(false);

	thread.Create(this, true);
}

ThreadedClient::~ThreadedClient()
{
	thread.Terminate();
	delete this->connection;
	this->connection = NULL;
	this->recvPostBox = NULL;

	if(sendPostBox != NULL)
	{
		delete sendPostBox;
		this->sendPostBox = NULL;
	}
}

void ThreadedClient::Send(CustomNetProtocol* protocol)
{
	this->sendPostBox->PostMessage(protocol);
}

bool ThreadedClient::IsConnected()
{
	return connection->IsConnected();
}

void ThreadedClient::Disconnect()
{
	connection->Disconnect();
}

int ThreadedClient::Send()
{
	int errorCode = 0;

	if(sendPostBox->IsFull())
	{
		OysterByte temp;
		sendPostBox->FetchMessage();
		//errorCode = this->connection->Send(temp);
	}

	return errorCode;
}

int ThreadedClient::Recv()
{
	int errorCode = -1;
	
	OysterByte temp;
	errorCode = this->connection->Recieve(temp);

	if(errorCode == 0)
	{
		stdMutex.lock();
		//recvPostBox->PostMessage(temp);
		stdMutex.unlock();
	}

	return errorCode;
}

void ThreadedClient::ThreadEntry()
{
	std::cout<< "Client Thread started" << std::endl;
}

Oyster::Thread::IThreadObject::ThreadCode ThreadedClient::ThreadExit()
{
	std::cout << "Client Thread exit" << std::endl;
	return Oyster::Thread::IThreadObject::ThreadCode_Exit;
}

#include <Windows.h>

bool ThreadedClient::DoWork()
{
	int errorCode;
	errorCode = Send();
	/*if(errorCode != 0)
	{
		return false;
	}*/

	errorCode = Recv();
	/*if(errorCode != 0)
	{
		return false;
	}*/
	Sleep(1);
	return true;
}

int ThreadedClient::Connect(unsigned short port, const char serverName[])
{
	int errorCode;

	if((errorCode = connection->InitiateClient()) != 0)
	{
		return errorCode;
	}

	else if((errorCode = connection->Connect(port, serverName)) != 0)
	{
		return errorCode;
	}

	thread.Create(this, true);
	return 0;
}

void ThreadedClient::setRecvPostBox(IPostBox<CustomNetProtocol*> *postBox)
{
	stdMutex.lock();
	this->recvPostBox = postBox;
	stdMutex.unlock();
}