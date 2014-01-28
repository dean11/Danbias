#ifndef INCLUDE_WINSOCK_LIB
#define INCLUDE_WINSOCK_LIB
	#pragma comment(lib, "ws2_32.lib")
#endif

#include "NetworkClient.h"

#include "Translator.h"
#include "CustomNetProtocol.h"
#include "NetworkSession.h"

#include "../NetworkDependencies/Connection.h"
#include "../NetworkDependencies/PostBox.h"
#include "../NetworkDependencies/WinsockFunctions.h"

#include "../../Misc/Utilities.h"
#include "../../Misc/Thread/IThreadObject.h"
#include "../../Misc/Thread/OysterThread.h"

using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace Utility::DynamicMemory;
using namespace Utility::Container;

/*************************************
			PrivateData
*************************************/
typedef NetworkClient::ClientEventArgs CEA;
struct NetDataContainer : public IThreadObject
{ //This struct is contained within a smart pointer. To avoide dependencies in link its implemented here..
	NetworkSession *owner;
	NetworkClient *parent;
	Connection connection;
	Translator translator;
	OysterThread thread;


	//Message queue for sending and recieving
	ThreadSafeQueue<CustomNetProtocol> sendQueue;
	ThreadSafeQueue<NetEvent<NetworkClient*, NetworkClient::ClientEventArgs>> recieveQueue;
	
	//ID
	static unsigned int currID;
	const unsigned int ID;

	NetDataContainer() 
		:	ID(currID++)
		,	parent(0)
		,	owner(0)
	{ 
		
		InitWinSock();
		this->thread.Create(this, true);
		this->thread.SetPriority(Oyster::Thread::OYSTER_THREAD_PRIORITY_1);
	}
	NetDataContainer(const NetDataContainer& obj)
		:ID(obj.ID) {  }
	~NetDataContainer()
	{ 
		ShutdownWinSock();
		this->connection.Disconnect();
		this->thread.Terminate();
		this->owner = 0;
		this->parent = 0;
	}

	bool DoWork() override
	{
		if(!this->connection.IsConnected())	return false;
		
		Send();
		Recv();
		
		return true;
	}

	int Send()
	{
		int errorCode = 0;

		if(!this->sendQueue.IsEmpty())
		{
			SmartPointer<OysterByte> temp = new OysterByte();
			CustomNetProtocol p = this->sendQueue.Pop();
			this->translator.Pack(temp, p);
			errorCode = this->connection.Send(temp);
			if(errorCode != 0)
			{
				CEA parg;
				parg.type = CEA::EventType_ProtocolFailedToSend;
				parg.data.protocol = p;
				NetEvent<NetworkClient*, CEA> e = { this->parent, parg };
				this->recieveQueue.Push(e);
			}
		}

		return errorCode;
	}

	int Recv()
	{
		int errorCode = -1;

		OysterByte temp = OysterByte();
		errorCode = this->connection.Recieve(temp);
		
		if(errorCode == 0 && temp.GetSize())
		{
			CustomNetProtocol protocol;
			bool ok = this->translator.Unpack(protocol, temp);
		
			//Check if the protocol was unpacked correctly
			if(ok)
			{
				CEA parg;
				parg.type = CEA::EventType_ProtocolRecieved;
				parg.data.protocol = protocol;
				NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e = { this->parent, parg };
				this->recieveQueue.Push(e);
			}
		}
		else
		{
			CEA parg;
			parg.type = CEA::EventType_ProtocolFailedToRecieve;
			parg.data.nothing = 0;
			NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e = { this->parent, parg };
			this->recieveQueue.Push(e);
		}
	
		return errorCode;
	}
};


struct NetworkClient::PrivateData
{
	SmartPointer<NetDataContainer> dat;

public:
	PrivateData() 
	{ this->dat = new NetDataContainer(); }
	PrivateData(const PrivateData& obj)
	{ this->dat = obj.dat; }
	~PrivateData()
	{ this->dat = 0; }
};
unsigned int NetDataContainer::currID = 0;

/*************************************
			NetworkClient
*************************************/

NetworkClient::NetworkClient()
	:	privateData(0)
{  }

NetworkClient::NetworkClient(const NetworkClient& obj)
{
	if(obj.privateData) this->privateData = new PrivateData(*obj.privateData);
	else				this->privateData = 0;
}

NetworkClient& NetworkClient::operator =(const NetworkClient& obj)
{
	delete privateData;
	this->privateData = 0;
	if(obj.privateData) this->privateData = new PrivateData(*obj.privateData);

	return *this;
}

NetworkClient::~NetworkClient()
{
	if(this->privateData)
	{
		delete this->privateData;
		this->privateData = NULL;
	}
}

bool NetworkClient::operator ==(const NetworkClient& obj)
{
	return (this->privateData->dat->ID == obj.privateData->dat->ID);
}

bool NetworkClient::operator ==(const int& ID)
{
	return this->privateData->dat->ID == ID;
}

void NetworkClient::ProcessMessages()
{
	while (!this->privateData->dat->recieveQueue.IsEmpty())
	{
		if(this->privateData->dat->owner)
		{
			this->privateData->dat->owner->ClientEventCallback(this->privateData->dat->recieveQueue.Pop());
		}
	}
}

bool NetworkClient::Connect(int socket)
{
	if(this->IsConnected()) return true;
	if(this->privateData)	return false;
	if(!this->privateData)	this->privateData = new PrivateData();

	int result = this->privateData->dat->connection.Connect(socket, true);
	
	//Connect has succeeded
	if(result == 0)		return true;

	//Connect has failed
	return false;
}

bool NetworkClient::Connect(unsigned short port, const char serverIP[])
{
	if(this->IsConnected())	return false;
	if(this->privateData)	return false;
	if(!this->privateData)	this->privateData = new PrivateData();
	
	int result = this->privateData->dat->connection.Connect(port, serverIP, false);
	
	//Connect has succeeded
	if(result == 0)		return true;

	//Connect has failed
	return false;
}

void NetworkClient::Disconnect()
{
	privateData->dat->connection.Disconnect();
	privateData->dat->thread.Terminate();
}

void NetworkClient::Send(CustomProtocolObject& protocol)
{
	this->privateData->dat->sendQueue.Push(*protocol.GetProtocol());
}

void NetworkClient::Send(CustomNetProtocol* protocol)
{
	this->privateData->dat->sendQueue.Push(*protocol);
}

void NetworkClient::SetOwner(NetworkSession* owner)
{
	this->privateData->dat->owner = owner;
}

bool NetworkClient::IsConnected()
{
	if(!this->privateData) return false;
	return privateData->dat->connection.IsConnected();
}

int NetworkClient::GetID() const
{
	return this->privateData->dat->ID;
}



