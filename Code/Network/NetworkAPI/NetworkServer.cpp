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
public:
	PrivateData()
		:	listener(0)
		,	mainSession(0)
		,	isInitiated(0)
		,	isReleased(0)
		,	isRunning(0)
	{  }
	~PrivateData()
	{  }

	bool DoWork();

public:
	IListener* listener;
	PostBox<int> postBox;	//Postbox for new clients
	OysterThread thread;	//Server thread
	NetworkSession *mainSession;
	Utility::Container::ThreadSafeQueue<NetworkClient> clientQueue;

	bool isInitiated;
	bool isReleased;
	bool isRunning;
	int port;
};

bool NetworkServer::PrivateData::DoWork()
{
	/** Check for new clients **/
	if(postBox.IsFull())
	{
		int clientSocketNum = postBox.FetchMessage();

		if(clientSocketNum == -1)
		{
			//Something went wrong somewhere... do we care?
		}

		
		Oyster::Network::NetworkClient client;
		client.Connect(clientSocketNum);
		if(this->mainSession)
			this->clientQueue.Push(client);
	}

	return true;
}

/*************************************
			NetworkServer
*************************************/

NetworkServer::NetworkServer()
{
	this->privateData = new PrivateData();
}
NetworkServer::NetworkServer(const NetworkServer& obj)
{
	delete this->privateData;
	this->privateData = new PrivateData(*obj.privateData);
}
const NetworkServer& NetworkServer::operator=(const NetworkServer& obj)
{
	delete this->privateData;
	this->privateData = new PrivateData(*obj.privateData);
	return *this;
}
NetworkServer::~NetworkServer()
{
	if(this->privateData)
	{
		delete this->privateData;
		this->privateData = 0;
	}
}

NetworkServer::ServerReturnCode NetworkServer::Init(const int& port, NetworkSession const* mainSession)
{
	//Check if it's a valid port
	if(port == 0 || port == -1)
	{
		return NetworkServer::ServerReturnCode_Error;
	}
	else if(this->privateData->port != 0 && this->privateData->port != -1)
	{
		return NetworkServer::ServerReturnCode_Error;
	}
	if(!InitWinSock())
	{
		return NetworkServer::ServerReturnCode_Error;
	}

	//Initiate listener
	this->privateData->listener = new Listener(&this->privateData->postBox);
	if(!((Listener*)this->privateData->listener)->Init(port, false))
	{
		return NetworkServer::ServerReturnCode_Error;
	}

	if(this->privateData->thread.Create(this->privateData, false) == OYSTER_THREAD_ERROR_FAILED)
	{
		return NetworkServer::ServerReturnCode_Error;
	}

	this->privateData->isInitiated = true;
	this->privateData->isReleased = false;
	this->privateData->mainSession = 0;
	return NetworkServer::ServerReturnCode_Sucess;
}

NetworkServer::ServerReturnCode NetworkServer::Start()
{
	//Start listener
	if(!((Listener*)this->privateData->listener)->Start())
	{
		return NetworkServer::ServerReturnCode_Error;
	}

	if(this->privateData->thread.Start() == OYSTER_THREAD_ERROR_FAILED)
	{
		return NetworkServer::ServerReturnCode_Error;
	}

	this->privateData->isRunning = true;
	return NetworkServer::ServerReturnCode_Sucess;
}

void NetworkServer::Stop()
{
	if(this->privateData->listener)
	{
		((Listener*)this->privateData->listener)->Stop();
	}

	this->privateData->thread.Stop();

	this->privateData->isRunning = false;
}

void NetworkServer::Shutdown()
{
	if(this->privateData->listener)
	{
		this->privateData->listener->Shutdown();
		delete this->privateData->listener;
		this->privateData->listener = NULL;
	}
	
	this->privateData->thread.Terminate();

	ShutdownWinSock();

	this->privateData->isRunning = false;
	this->privateData->mainSession = 0;
	this->privateData->isReleased = true;
}

void NetworkServer::ProcessConnectedClients()
{
	while(!this->privateData->clientQueue.IsEmpty())
	{
		if(this->privateData->mainSession)	this->privateData->mainSession->Attach(this->privateData->clientQueue.Pop());
	}
}

void NetworkServer::SetSession(NetworkSession const* mainSession)
{
	this->privateData->mainSession = const_cast<NetworkSession*>(mainSession);
}

NetworkSession const* NetworkServer::GetMainSession()
{
	return this->privateData->mainSession;
}

NetworkSession const* NetworkServer::ReleaseMainSessionSession()
{
	NetworkSession const *  temp;
	temp = this->privateData->mainSession;
	this->privateData->mainSession = 0;
	return temp;
}

bool NetworkServer::IsStarted() const
{
	return this->privateData->isRunning;
}






