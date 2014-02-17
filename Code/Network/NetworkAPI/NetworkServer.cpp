//////////////////////////////////////
// Created by Pontus Fransson 2013  //
// Modified by Dennis Andersen 2014 //
//////////////////////////////////////

#ifndef INCLUDE_WINSOCK_LIB
#define INCLUDE_WINSOCK_LIB
	#pragma comment(lib, "ws2_32.lib")
#endif

#include "NetworkServer.h"

#include "../NetworkDependencies/Listener.h"
#include "../NetworkDependencies/PostBox.h"
#include "../NetworkDependencies/WinsockFunctions.h"

#include "Utilities.h"
#include "Thread/OysterThread.h"
#include "WinTimer.h"


using namespace Oyster::Network;
using namespace Utility::DynamicMemory;
using namespace Oyster::Thread;

/*************************************
			PrivateData
*************************************/

void Broadcast()
{
	char pkt[4];
	size_t pkt_length = 4;
	sockaddr_in dest;
	sockaddr_in local;
	WSAData data;
	WSAStartup( MAKEWORD( 2, 2 ), &data );

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	local.sin_port = 15151; // choose any

	dest.sin_family = AF_INET;
	dest.sin_port = htons( 15151 );

	// create the socket
	SOCKET s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	// bind to the local address
	bind( s, (sockaddr *)&local, sizeof(local) );

	std::string addr;
	for (int i = 0; i < 256; i++)
	{
		addr = "192.168.0.";
		char buff[5];
		_itoa_s<5>(i, buff, 10);

		addr.append(buff);
		dest.sin_addr.s_addr = inet_addr( addr.c_str() );
		// send the pkt
		int ret = sendto( s, pkt, pkt_length, 0, (sockaddr *)&dest, sizeof(dest) );
	}
}

struct TimeInstance
{
	float length;
	float previous;
	TimeInstance() :length(0.0f), previous(0.0f) {}
	TimeInstance(float length, float previous) :length(length), previous(previous) {}
};
struct NetworkServer::PrivateData : public IThreadObject
{
public:
	PrivateData()
		:	listener(0)
		,	mainSession(0)
		,	isInitiated(0)
		,	isReleased(0)
		,	isRunning(0)
		,	port(-1)
		,	broadcast(0)
		,	broadcastTime(1.0f, 0.0f)
	{  }
	~PrivateData()
	{  }

	bool DoWork();

public:
	Listener* listener;
	PostBox<ConnectionInfo> postBox;	//Postbox for new clients
	OysterThread thread;	//Server thread
	NetworkSession *mainSession;
	Utility::Container::ThreadSafeQueue<SmartPointer<NetworkClient>> clientQueue;

	bool isInitiated;
	bool isReleased;
	bool isRunning;
	int port;
	bool broadcast;

	TimeInstance broadcastTime;

	Utility::WinTimer serverTimer;
};

bool NetworkServer::PrivateData::DoWork()
{
	if(broadcast)	
	{
		if( (this->serverTimer.getElapsedSeconds() - this->broadcastTime.previous) >= this->broadcastTime.length )
		{
			Broadcast();
		}
	}

	/** Check for new clients **/
	if(postBox.IsFull())
	{
		ConnectionInfo clientSocketNum = postBox.FetchMessage();
		
		if(clientSocketNum.socket == -1)
		{
			//Something went wrong somewhere... do we care?
		}
		
		SmartPointer<NetworkClient> client(new NetworkClient());
		client->Connect(clientSocketNum);
		
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

NetworkServer::ServerReturnCode NetworkServer::Init(ServerOptions& options)
{
	this->privateData->mainSession = const_cast<NetworkSession*>(options.mainOptions.ownerSession);
	//Check if it's a valid port
	if(options.mainOptions.listenPort == 0 || options.mainOptions.listenPort == -1)
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
	if(!this->privateData->listener->Init(options.mainOptions.listenPort, false))
	{
		return NetworkServer::ServerReturnCode_Error;
	}

	if(this->privateData->thread.Create(this->privateData, false) == OYSTER_THREAD_ERROR_FAILED)
	{
		return NetworkServer::ServerReturnCode_Error;
	}

	this->privateData->isInitiated = true;
	this->privateData->isReleased = false;
	return NetworkServer::ServerReturnCode_Sucess;
}

NetworkServer::ServerReturnCode NetworkServer::Start()
{
	this->privateData->serverTimer.reset();
	//Start listener
	if(!this->privateData->listener->Start())
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
		this->privateData->listener->Stop();
	}

	this->privateData->thread.Stop();

	this->privateData->isRunning = false;
}

void NetworkServer::Shutdown()
{
	if(this->privateData->mainSession)
	{
		this->privateData->mainSession->CloseSession(true);
	}
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

int NetworkServer::Update()
{
	int c = 0;
	while(!this->privateData->clientQueue.IsEmpty())
	{
		SmartPointer<NetworkClient> client = this->privateData->clientQueue.Pop();

		if(this->privateData->mainSession)
		{
			this->privateData->mainSession->ClientConnectedEvent(client);
			c++;
		}
		else
		{
			//Clients have nowhere to go?
			client->Disconnect();
		}
	}
	return c;
}

void NetworkServer::SetSession(NetworkSession const* mainSession)
{
	this->privateData->mainSession = const_cast<NetworkSession*>(mainSession);
}

NetworkSession const* NetworkServer::GetMainSession()
{
	return this->privateData->mainSession;
}

NetworkSession const* NetworkServer::ReleaseMainSession()
{
	NetworkSession const *  temp;
	temp = this->privateData->mainSession;
	this->privateData->mainSession = 0;
	return temp;
}

bool NetworkServer::IsRunning() const
{
	return this->privateData->isRunning;
}

std::string NetworkServer::GetLanAddress()
{
	std::string szLocalIP;
	char szHostName[255];
	struct hostent *host_entry;

	gethostname(szHostName, 255);
	
	host_entry = gethostbyname(szHostName);
	char* temp = inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list);

	char buff[255];
	strcpy_s(buff, temp);
	szLocalIP = buff;
	return szLocalIP;
}
int NetworkServer::GetPort()
{
	return this->privateData->port;
}




