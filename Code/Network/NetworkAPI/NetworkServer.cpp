#ifndef INCLUDE_WINSOCK_LIB
#define INCLUDE_WINSOCK_LIB
	#pragma comment(lib, "ws2_32.lib")
#endif

#include "NetworkServer.h"

#include "../NetworkDependencies/Listener.h"
#include "../NetworkDependencies/PostBox.h"
#include "../NetworkDependencies/WinsockFunctions.h"

#include "../../Misc/Utilities.h"
#include "../../Misc/Thread/OysterThread.h"

using namespace Oyster::Network;
using namespace ::Server;
using namespace Utility::DynamicMemory;
using namespace Oyster::Thread;

/*************************************
			PrivateData
*************************************/

struct NetworkServer::PrivateData : public IThreadObject
{
	PrivateData();
	~PrivateData();

	bool Init(INIT_DESC& initDesc);
	bool Start();
	void Stop();
	void Shutdown();

	void CheckForNewClient();

	bool DoWork();

	//
	IListener* listener;
	INIT_DESC initDesc;
	bool started;

	//Postbox for new clients
	IPostBox<int> *postBox;

	//Server thread
	OysterThread thread;

};

NetworkServer::PrivateData::PrivateData()
{
	listener = 0;
	started = false;
	postBox = new PostBox<int>;
}

NetworkServer::PrivateData::~PrivateData()
{
	Shutdown();
}

bool NetworkServer::PrivateData::Init(INIT_DESC& initDesc)
{
	//Check if it's a valid port
	if(initDesc.port == 0)
	{
		return false;
	}

	if(!InitWinSock())
		return false;

	this->initDesc = initDesc;

	//Initiate listener
	listener = new Listener(postBox);
	if(!((Listener*)listener)->Init(this->initDesc.port, false))
	{
		return false;
	}

	if(thread.Create(this, false) == OYSTER_THREAD_ERROR_FAILED)
	{
		return false;
	}

	return true;
}

bool NetworkServer::PrivateData::Start()
{
	//Start listener
	if(!((Listener*)listener)->Start())
	{
		return false;
	}

	started = true;

	if(thread.Start() == OYSTER_THREAD_ERROR_FAILED)
	{
		return false;
	}

	return true;
}

void NetworkServer::PrivateData::Stop()
{
	if(listener)
	{
		((Listener*)listener)->Stop();
	}

	started = false;

	thread.Stop();
}

void NetworkServer::PrivateData::Shutdown()
{
	//Stop server main thread
	thread.Stop();

	if(listener)
	{
		delete listener;
		listener = NULL;
	}

	if(postBox)
	{
		delete postBox;
		postBox = NULL;
	}

	started = false;

	ShutdownWinSock();
}

//Checks for new clients and sends them to the proc function.
void NetworkServer::PrivateData::CheckForNewClient()
{
	if(postBox->IsFull())
	{
		int clientSocketNum = postBox->FetchMessage();

		//Safety check that is probably not needed.
		if(clientSocketNum == -1)
		{
			return;
		}

		//Create client and Proc function if the pointer is not NULL
		if(initDesc.callbackType == NetworkClientCallbackType_Function)
		{
			Oyster::Network::NetworkClient *client = new Oyster::Network::NetworkClient(clientSocketNum);
			initDesc.recvObj.clientConnectFnc(client);
		}
		else if(initDesc.callbackType == NetworkClientCallbackType_Object)
		{
			Oyster::Network::NetworkClient *client = new Oyster::Network::NetworkClient(clientSocketNum);
			initDesc.recvObj.clientConnectObject->ClientConnectCallback(client);
		}
	}
}

bool NetworkServer::PrivateData::DoWork()
{
	CheckForNewClient();

	return true;
}

/*************************************
			NetworkServer
*************************************/

NetworkServer::NetworkServer()
{
	privateData = new PrivateData();
}

NetworkServer::~NetworkServer()
{
	if(privateData)
	{
		delete privateData;
	}
}

bool NetworkServer::Init(INIT_DESC& initDesc)
{
	if(!privateData->Init(initDesc))
	{
		return false;
	}

	return true;
}

bool NetworkServer::Start()
{
	if(!privateData->Start())
	{
		return false;
	}

	return true;
}

void NetworkServer::Stop()
{
	privateData->Stop();
}

void NetworkServer::Shutdown()
{
	privateData->Shutdown();
}

bool NetworkServer::IsStarted() const
{
	return privateData->started;
}