#include "NetworkServer.h"
#include "NetworkClient.h"

#include "../NetworkDependencies/Listener.h"
#include "../NetworkDependencies/PostBox.h"

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
	bool Stop();
	bool Shutdown();

	void CheckForNewClient();

	virtual bool DoWork();

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

	this->initDesc = initDesc;

	//Initiate listener
	listener = new Listener(postBox);
	((Listener*)listener)->Init(this->initDesc.port, false);

	thread.Create(this, false);

	return true;
}

bool NetworkServer::PrivateData::Start()
{
	//Start listener
	((Listener*)listener)->Start();
	started = true;

	thread.Start();

	return true;
}

bool NetworkServer::PrivateData::Stop()
{
	if(listener)
	{
		((Listener*)listener)->Stop();
	}

	started = false;

	thread.Stop();

	return true;
}

bool NetworkServer::PrivateData::Shutdown()
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

	return true;
}

//Checks for new clients and sends them to the proc function.
void NetworkServer::PrivateData::CheckForNewClient()
{
	if(postBox->IsFull())
	{
		int clientSocketNum;
		postBox->FetchMessage(clientSocketNum);

		//Safety check that is probably not needed.
		if(clientSocketNum == -1)
		{
			return;
		}

		//Create client and Proc function if the pointer is not NULL
		if(initDesc.proc)
		{
			Oyster::Network::NetworkClient* client = new Oyster::Network::NetworkClient();
			initDesc.proc((NetworkClient*)client);
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
	privateData->Init(initDesc);

	return true;
}

bool NetworkServer::Start()
{
	privateData->Start();

	return true;
}

bool NetworkServer::Stop()
{
	privateData->Stop();

	return true;
}

bool NetworkServer::Shutdown()
{
	privateData->Shutdown();

	return true;
}

bool NetworkServer::IsStarted() const
{
	return privateData->started;
}