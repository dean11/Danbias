#include "IServer.h"
#include "../NetworkDependencies/Listener.h"
#include "IClient.h"
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

struct IServer::PrivateData : public IThreadObject
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

IServer::PrivateData::PrivateData()
{
	listener = 0;
	started = false;
	postBox = new PostBox<int>;
}

IServer::PrivateData::~PrivateData()
{
	Shutdown();
}

bool IServer::PrivateData::Init(INIT_DESC& initDesc)
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

bool IServer::PrivateData::Start()
{
	//Start listener
	((Listener*)listener)->Start();
	started = true;

	thread.Start();

	return true;
}

bool IServer::PrivateData::Stop()
{
	if(listener)
	{
		((Listener*)listener)->Stop();
	}

	started = false;

	thread.Stop();

	return true;
}

bool IServer::PrivateData::Shutdown()
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
void IServer::PrivateData::CheckForNewClient()
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

		//Create the new client
		IClient* client = new IClient();
		initDesc.proc(client);
	}
}

bool IServer::PrivateData::DoWork()
{
	CheckForNewClient();

	return true;
}

/*************************************
			IServer
*************************************/

IServer::IServer()
{
	privateData = new PrivateData();
}

IServer::~IServer()
{
	if(privateData)
	{
		delete privateData;
	}
}

bool IServer::Init(INIT_DESC& initDesc)
{
	privateData->Init(initDesc);

	return true;
}

bool IServer::Start()
{
	privateData->Start();

	return true;
}

bool IServer::Stop()
{
	privateData->Stop();

	return true;
}

bool IServer::Shutdown()
{
	privateData->Shutdown();

	return true;
}

bool IServer::IsStarted() const
{
	return privateData->started;
}