#include "ThreadedClient.h"
#include "OysterByte.h"

#include <iostream>
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace Utility::DynamicMemory;

ThreadedClient::ThreadedClient()
{
	this->connection = new Connection();
	this->sendPostBox = new PostBox<SmartPointer<OysterByte>>();
	this->recvPostBox = NULL;

	connection->SetBlockingMode(false);
}

ThreadedClient::ThreadedClient(unsigned int socket)
{
	this->connection = new Connection(socket);
	this->sendPostBox = new PostBox<SmartPointer<OysterByte>>();
	this->recvPostBox = NULL;

	connection->SetBlockingMode(false);

	thread.Create(this, true);
}

ThreadedClient::ThreadedClient(IPostBox<Utility::DynamicMemory::SmartPointer<OysterByte>>* postBox, unsigned int socket)
{
	this->connection = new Connection(socket);
	this->sendPostBox = new PostBox<SmartPointer<OysterByte>>;
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

void ThreadedClient::Send(SmartPointer<OysterByte>& byte)
{
	this->sendPostBox->PostMessage(byte);
}

int ThreadedClient::Send()
{
	int errorCode = 0;

	if(sendPostBox->IsFull())
	{
		SmartPointer<OysterByte> temp = new OysterByte;
		sendPostBox->FetchMessage(temp);
		errorCode = this->connection->Send(temp);
	}

	return errorCode;
}

int ThreadedClient::Recv()
{
	int errorCode = -1;
	
	SmartPointer<OysterByte> temp = new OysterByte;
	errorCode = this->connection->Recieve(temp);
	
	if(errorCode == 0)
	{
		stdMutex.lock();
		recvPostBox->PostMessage(temp);
		stdMutex.unlock();
	}

	return errorCode;
}

void ThreadedClient::ThreadEntry()
{
	std::cout<< "Client Thread started" << std::endl;
}

void ThreadedClient::ThreadExit()
{
	std::cout << "Client Thread exit" << std::endl;
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

void ThreadedClient::setRecvPostBox(IPostBox<SmartPointer<OysterByte>> *postBox)
{
	stdMutex.lock();
	this->recvPostBox = postBox;
	stdMutex.unlock();
}