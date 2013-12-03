#include "ThreadedClient.h"

#include <iostream>
using namespace Oyster::Network;
using namespace Oyster::Thread;

ThreadedClient::ThreadedClient()
{
	this->connection = new Connection();
	this->sendPostBox = new PostBox<OysterByte*>();
	this->recvPostBox = NULL;
}

ThreadedClient::ThreadedClient(unsigned int socket)
{
	this->connection = new Connection(socket);
	this->sendPostBox = new PostBox<OysterByte*>();
	this->recvPostBox = NULL;

	connection->SetBlockingMode(false);

	thread.Create(this, true);
}

ThreadedClient::ThreadedClient(IPostBox<OysterByte*>* postBox, unsigned int socket)
{
	this->connection = new Connection(socket);
	this->sendPostBox = new PostBox<OysterByte*>;
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

int ThreadedClient::Send(OysterByte* byte)
{
	mutex.LockMutex();
	this->sendPostBox->PostMessage(byte);
	mutex.UnlockMutex();
	return 0;
}

int ThreadedClient::Send()
{
	int errorCode = 0;
	mutex.LockMutex();
	if(sendPostBox->IsFull())
	{
		OysterByte *temp = NULL;
		sendPostBox->FetchMessage(temp);
		errorCode = this->connection->Send(*temp);
	}
	mutex.UnlockMutex();

	return errorCode;
}

int ThreadedClient::Recv()
{
	int errorCode = 0;

	OysterByte *temp = new OysterByte();
	errorCode = this->connection->Recieve(*temp);
	
	if(errorCode == 0)
	{
		mutex.LockMutex();
		recvPostBox->PostMessage(temp);
		mutex.UnlockMutex();
	}
	else
	{
		delete temp;
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

void ThreadedClient::setRecvPostBox(IPostBox<OysterByte*>* postBox)
{
	this->mutex.LockMutex();
	this->recvPostBox = postBox;
	this->mutex.UnlockMutex();
}