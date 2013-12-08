#include "IServer.h"
#include "../NetworkDependencies/Listener.h"
#include "IClient.h"
#include "../NetworkDependencies/PostBox.h"
#include "../../Misc/Utilities.h"

using namespace Oyster::Network;
using namespace ::Server;
using namespace Utility::DynamicMemory;

/*************************************
			PrivateData
*************************************/

struct IServer::PrivateData
{
	PrivateData();
	~PrivateData();

	bool Init(INIT_DESC& initDesc);
	bool Start();
	bool Stop();
	bool Shutdown();
	
	//
	IListener* listener;
	INIT_DESC initDesc;
	bool started;

	IPostBox<SmartPointer<int>> *postBox;
};

IServer::PrivateData::PrivateData()
{
	listener = 0;
	started = false;
	postBox = new PostBox<SmartPointer<int>>();
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
	listener = new Listener();
	((Listener*)listener)->Init(this->initDesc.port, false);

	return true;
}

bool IServer::PrivateData::Start()
{
	//Start listener
	((Listener*)listener)->Start();
	started = true;

	return true;
}

bool IServer::PrivateData::Stop()
{
	if(listener)
	{
		((Listener*)listener)->Stop();
	}

	started = false;

	return true;
}

bool IServer::PrivateData::Shutdown()
{
	if(listener)
	{
		delete listener;
		listener = NULL;
	}

	started = false;

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

void IServer::AddSession(ISession* session)
{

}

void IServer::RemoveSession(ISession* session)
{

}

bool IServer::IsStarted() const
{
	return privateData->started;
}