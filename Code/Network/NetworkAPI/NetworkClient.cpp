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
#include "../../Misc/Packing/Packing.h"

#include <queue>

using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace Utility::DynamicMemory;
using namespace Utility::Container;
using namespace std;

/*************************************
			PrivateData
*************************************/
typedef NetworkClient::ClientEventArgs CEA;

struct NetworkClient::PrivateData : public IThreadObject
{
	NetworkSession *owner;
	NetworkClient *parent;
	Connection connection;
	Translator translator;
	OysterThread thread;

	OysterByte recieveBuffer;

	//Message queue for sending and recieving
	ThreadSafeQueue<CustomNetProtocol> sendQueue;
	ThreadSafeQueue<NetEvent<NetworkClient*, NetworkClient::ClientEventArgs>> recieveQueue;

	//ID
	static unsigned int currID;
	const unsigned int ID;

	PrivateData() 
		:	ID(currID++)
		,	parent(0)
		,	owner(0)
	{ 
		
		InitWinSock();
		this->thread.Create(this, false);
		this->thread.SetPriority(Oyster::Thread::OYSTER_THREAD_PRIORITY_1);
	}
	~PrivateData()
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
			HandleRecievedData(temp);


			/*	Replaced with EmptyOutbufferedQueue() and HandleRecievedData(OysterByte)
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
			}*/

		}
		//else
		//{
		//	CEA parg;
		//	parg.type = CEA::EventType_ProtocolFailedToRecieve;
		//	parg.data.nothing = 0;
		//	NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e = { this->parent, parg };
		//	this->recieveQueue.Push(e);
		//}
	
		return errorCode;
	}

	void HandleRecievedData(OysterByte& data)
	{
		//Loop through all packages that was recieved and add them to the queue.
		unsigned int size = 0;

		Oyster::Network::OysterByte msg;

		//If there is part of a message in the buffer.
		if(recieveBuffer.GetSize() > 0)
		{
			//cout << "the buffer size: " << recvBuffer.GetSize() <<endl;
			unsigned int temp = recieveBuffer.GetSize();
			size = Oyster::Packing::Unpacki(recieveBuffer);

			if(temp + data.GetSize() > size)
			{
				msg = recieveBuffer;
				recieveBuffer.Clear();
				size -= msg.GetSize();
				msg.AppendPartOfArray(data, 0, size);
				UnpackMessageAndAddToQueue(msg);
			}
			else if(temp + data.GetSize() == size)
			{
				msg = recieveBuffer;
				recieveBuffer.Clear();
				size -= msg.GetSize();
				msg += data;
				UnpackMessageAndAddToQueue(msg);
				return;
			}
			else
			{
				recieveBuffer += data;
				size = data.GetSize();
				return;
			}
		}

 		for(unsigned int i = size; i < data.GetSize(); i += size)
		{
			size = Oyster::Packing::Unpacki(&data.GetByteArray()[i]);
			if(i+size > data.GetSize())
			{
				//Add it to the recvBuffer instead.
				recieveBuffer.AppendPartOfArray(data, i, data.GetSize());
				break;
			}
			msg.Clear();
			msg.AppendPartOfArray(data, i, i+size);
			UnpackMessageAndAddToQueue(msg);
		}
	}

	void UnpackMessageAndAddToQueue(OysterByte& msg)
	{
		CustomNetProtocol protocol;
		bool ok = this->translator.Unpack(protocol, msg);

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
};
unsigned int NetworkClient::PrivateData::currID = 0;

/*************************************
			NetworkClient
*************************************/

NetworkClient::NetworkClient()
	:	privateData(0)
{  }

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
	return (this->privateData->ID == obj.privateData->ID);
}

bool NetworkClient::operator ==(const int& ID)
{
	return this->privateData->ID == ID;
}

void NetworkClient::Update()
{
	if(!this->privateData) return;
	while (!this->privateData->recieveQueue.IsEmpty())
	{
		NetEvent<NetworkClient*, ClientEventArgs> temp = this->privateData->recieveQueue.Pop();

		this->DataRecieved(temp);

	//--------- Deprecate --------- 
		this->NetworkCallback(temp.args.data.protocol);
	//------------------------------
	}
}

bool NetworkClient::Connect(ConnectionInfo& socket)
{
	if(this->IsConnected()) return true;
	if(this->privateData)	return false;
	if(!this->privateData)	this->privateData = new PrivateData();

	int result = this->privateData->connection.Connect(socket, false);
	
	//Connect has succeeded
	if(result != 0)		return false;

	this->privateData->parent = this;
	this->privateData->thread.Start();

	//Connect has failed
	return true;
}

bool NetworkClient::Connect(unsigned short port, const char serverIP[])
{
	if(this->IsConnected())	return false;
	if(this->privateData)	return false;
	if(!this->privateData)	this->privateData = new PrivateData();
	
	int result = this->privateData->connection.Connect(port, serverIP, false);
	
	//Connect has succeeded
	if(result != 0)		return false;
	this->privateData->owner = 0;
	this->privateData->parent = this;
	this->privateData->thread.Start();

	//Connect has failed
	return true;
}

void NetworkClient::Disconnect()
{
	if(!privateData) return;

	privateData->connection.Disconnect();
	privateData->thread.Terminate();
}

void NetworkClient::Send(CustomProtocolObject& protocol)
{
	this->privateData->sendQueue.Push(*protocol.GetProtocol());
}

void NetworkClient::Send(CustomNetProtocol* protocol)
{
	this->privateData->sendQueue.Push(*protocol);
}

void NetworkClient::SetOwner(NetworkSession* owner)
{
	this->privateData->owner = owner;
}

bool NetworkClient::IsConnected()
{
	if(!this->privateData) return false;
	return privateData->connection.IsConnected();
}

int NetworkClient::GetID() const
{
	return this->privateData->ID;
}

void NetworkClient::DataRecieved(NetEvent<NetworkClient*, ClientEventArgs> e)
{
	if(this->privateData->owner)
	{
		this->privateData->owner->ClientEventCallback(e);
	}
}

void NetworkClient::NetworkCallback(Oyster::Network::CustomNetProtocol& p)
{}

std::string NetworkClient::GetIpAddress()
{
	return this->privateData->connection.GetIpAddress();
}
