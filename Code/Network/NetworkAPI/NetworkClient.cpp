//////////////////////////////////////
// Created by Pontus Fransson 2013  //
// Modified by Dennis Andersen 2014 //
//////////////////////////////////////
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

#include "Utilities.h"
#include "Thread/IThreadObject.h"
#include "Thread/OysterThread.h"
#include "Packing/Packing.h"

#include <queue>
#include <WinSock2.h>

//For conversion from wstring to string
#include <codecvt>

using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace Utility::DynamicMemory;
using namespace Utility::Container;
using namespace std;

/*************************************
			PrivateData
*************************************/
typedef NetworkClient::ClientEventArgs CEA;

void OnRecieve_Default(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e) {}

struct NetworkClient::PrivateData : public IThreadObject
{
	NetworkSession *owner;
	NetworkClient *parent;
	Connection connection;
	Translator translator;
	OysterThread thread;
	bool outputEvent;

	OysterByte recieveBuffer;

	//Message queue for sending and recieving
	ThreadSafeQueue<CustomNetProtocol> sendQueue;
	ThreadSafeQueue<NetEvent<NetworkClient*, NetworkClient::ClientEventArgs>> recieveQueue;

	//Testing for eventSelect.
	HANDLE socketEvents[2];
	HANDLE shutdownEvent;

	//The OysterByte each message is packed in.
	OysterByte tempMessage;

	//Used to buffer messages
	OysterByte bufferedSend;
	int numPackages;

	//ID
	static unsigned int currID;
	const unsigned int ID;

	PrivateData() 
		:	ID(currID++)
		,	parent(0)
		,	owner(0)
		,	outputEvent(0)
	{
		numPackages = 0;
		bufferedSend.Resize(MAX_NETWORK_MESSAGE_SIZE);
		tempMessage.Resize(MAX_NETWORK_MESSAGE_SIZE);
		InitWinSock();
		this->thread.Create(this, false);
		this->thread.SetPriority(Oyster::Thread::OYSTER_THREAD_PRIORITY_1);
	}
	~PrivateData()
	{ 
		SetEvent(shutdownEvent);
		this->thread.Wait();

		ShutdownWinSock();
		this->connection.Disconnect();
		this->owner = 0;
		this->parent = 0;
	}

	void ThreadEntry()
	{
		//Create alla events used in the thread
		shutdownEvent = CreateEvent(NULL, true, false, NULL);
		socketEvents[0] = WSACreateEvent();
		socketEvents[1] = WSACreateEvent();

		if(socketEvents[0] == WSA_INVALID_EVENT)
		{
			//Error
		}

		if(WSAEventSelect(this->connection.GetSocket(), socketEvents[0], FD_READ) == SOCKET_ERROR)
		{
			//Error
		}
	}

	void ThreadExit()
	{
		WSACloseEvent(socketEvents[0]);
		WSACloseEvent(socketEvents[1]);
		CloseHandle(shutdownEvent);
	}

	bool DoWork() override
	{
		WSANETWORKEVENTS wsaEvents;

		while(WaitForSingleObject(shutdownEvent, 0) != WAIT_OBJECT_0)
		{
			if(!this->connection.IsConnected())	return false;

			int result = WSAWaitForMultipleEvents(2, socketEvents, FALSE, 100, FALSE) - WSA_WAIT_EVENT_0;
			if(result == 0)
			{
				WSAEnumNetworkEvents(this->connection.GetSocket(), socketEvents[0], &wsaEvents);
				if((wsaEvents.lNetworkEvents & FD_READ) && (wsaEvents.iErrorCode[FD_READ_BIT] == 0))
				{
					//Recieve a message
					Recv();
				}
			}
			else if(result == 1)
			{
				//Send all messages in the sendQueue
				int i = this->sendQueue.Size();
				WSAResetEvent(socketEvents[1]);

				if(i == 1)
				{
					Send();
				}
				else if(i > 1)
				{
					for(int j = 0; j < i; j++)
						BufferMessage();

					SendBuffer();
				}
			}
		}

		return false;
	}

	void SendBuffer()
	{
		if(bufferedSend.GetSize() > 0)
		{
			this->connection.Send(bufferedSend);
			//printf("2. %d, %d\n", numPackages, bufferedSend.GetSize());
			bufferedSend.Clear();

			//Debug
			numPackages = 0;
		}
	}

	void BufferMessage()
	{
		int errorCode = 0;

		if(!this->sendQueue.IsEmpty())
		{
			CustomNetProtocol p = this->sendQueue.Pop();

			this->translator.Pack(tempMessage, p);


			if(tempMessage.GetSize() > MAX_NETWORK_MESSAGE_SIZE - bufferedSend.GetSize())
			{
				//Send buffered message
				errorCode = this->connection.Send(bufferedSend);
				//printf("2. %d, %d\n", numPackages, bufferedSend.GetSize());
				bufferedSend.Clear();

				//Debug
				numPackages = 0;
			}

			bufferedSend += tempMessage;
			tempMessage.Clear();

			//Debug
			numPackages++;

			if(errorCode != 0 && errorCode != WSAEWOULDBLOCK)
			{
				if( errorCode == WSAECONNABORTED || errorCode == WSAENOTCONN)
				{
					CEA parg;
					parg.type = CEA::EventType_Disconnect;
					parg.data.protocol = p;
					NetEvent<NetworkClient*, CEA> e = { this->parent, parg };
					this->recieveQueue.Push(e);
					
					if(this->outputEvent)
					{
						printf("\t(ID: %i | IP: %s | Protocol: %i) - EventType_Disconnect && EventType_ProtocolFailedToSend \n", this->ID, this->connection.GetIpAddress().c_str(), p[0].value.netShort);	
					}
				}
				else
				{
					CEA parg;
					parg.type = CEA::EventType_ProtocolFailedToSend;
					parg.data.protocol = p;
					NetEvent<NetworkClient*, CEA> e = { this->parent, parg };
					this->recieveQueue.Push(e);

					if(this->outputEvent)
					{
						printf("\t(ID: %i | IP: %s | Protocol: %i) - EventType_ProtocolFailedToSend\n", this->ID, this->connection.GetIpAddress().c_str(), p[0].value.netShort);	
					}
				}
			}

			if(this->outputEvent)
			{
				printf("\t(ID: %i | IP: %s | Protocol: %i) Message sent!\n", this->ID, this->connection.GetIpAddress().c_str(), p[0].value.netShort);	
			}
		}
	}

	int Send()
	{
		int errorCode = 0;

		if(!this->sendQueue.IsEmpty())
		{
			//printf("\t(%i)\n", this->sendQueue.Size());
			CustomNetProtocol p = this->sendQueue.Pop();

			this->translator.Pack(tempMessage, p);
 			errorCode = this->connection.Send(tempMessage);
			tempMessage.Clear();

			if(errorCode != 0 && errorCode != WSAEWOULDBLOCK)
			{
				if( errorCode == WSAECONNABORTED || errorCode == WSAENOTCONN)
				{
					CEA parg;
					parg.type = CEA::EventType_Disconnect;
					parg.data.protocol = p;
					NetEvent<NetworkClient*, CEA> e = { this->parent, parg };
					this->recieveQueue.Push(e);
					
					if(this->outputEvent)
					{
						printf("\t(ID: %i | IP: %s | Protocol: %i) - EventType_Disconnect && EventType_ProtocolFailedToSend \n", this->ID, this->connection.GetIpAddress().c_str(), p[0].value.netShort);	
					}
				}
				else
				{
					CEA parg;
					parg.type = CEA::EventType_ProtocolFailedToSend;
					parg.data.protocol = p;
					NetEvent<NetworkClient*, CEA> e = { this->parent, parg };
					this->recieveQueue.Push(e);

					if(this->outputEvent)
					{
						printf("\t(ID: %i | IP: %s | Protocol: %i) - EventType_ProtocolFailedToSend\n", this->ID, this->connection.GetIpAddress().c_str(), p[0].value.netShort);	
					}
				}
			}

			if(this->outputEvent)
			{
				printf("\t(ID: %i | IP: %s | Protocol: %i) Message sent!\n", this->ID, this->connection.GetIpAddress().c_str(), p[0].value.netShort);	
			}
		}

		return errorCode;
	}
	int Recv()
	{
		int errorCode = -1;

		errorCode = this->connection.Recieve(tempMessage);
		
		if(errorCode == 0 && tempMessage.GetSize())
		{
			HandleRecievedData(tempMessage);
			tempMessage.Clear();


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
			NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e;
			e.sender = this->parent;
			e.args.data.protocol = parg.data.protocol;
			e.args.type = parg.type;
			
			this->recieveQueue.Push(e);

			if(this->outputEvent)
			{
				printf("\t(ID: %i | IP: %s | Protocol: %i) Message recieved!\n", this->ID, this->connection.GetIpAddress().c_str(), protocol[0].value.netShort);	
			}
		}
		else
		{
			if(this->outputEvent)
			{
				printf("\t(ID: %i | IP: %s) Failed to unpack CustomNetProtocol!\n", this->ID, this->connection.GetIpAddress().c_str());	
			}
		}
	}
};
unsigned int NetworkClient::PrivateData::currID = 0;

/*************************************
			NetworkClient
*************************************/

NetworkClient::NetworkClient()
	:	privateData(nullptr),
		OnRecieve(OnRecieve_Default)
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
	}
}

bool NetworkClient::Connect(ConnectionInfo& socket)
{
	if(this->IsConnected()) return true;
	if(this->privateData)	return false;
	if(!this->privateData)	this->privateData = new PrivateData();

	int result = this->privateData->connection.Connect(socket, true);
	
	//Connect has succeeded
	if(result != 0)		return false;

	this->privateData->parent = this;
	this->privateData->thread.Start();

	//Connect has failed
	return true;
}

bool NetworkClient::Connect(unsigned short port, const char serverIP[])
{
	//Return true if you are already connected.
	if(this->IsConnected())
		return true;

	if(!this->privateData)
		this->privateData = new PrivateData();
	
	int result = this->privateData->connection.Connect(port, serverIP, true);
	
	//Connect has succeeded
	if(result != 0)		return false;
	this->privateData->owner = 0;
	this->privateData->parent = this;
	this->privateData->thread.Start();

	//Connect has failed
	return true;
}

bool NetworkClient::Connect(unsigned short port, std::wstring serverIP)
{
	//Convert from wstring to string.
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

	std::string ip = converterX.to_bytes(serverIP);

	return this->Connect(port, ip.c_str());
}

bool NetworkClient::Reconnect()
{
	//Return true if you are already connected.
	if(this->IsConnected())	
		return true;

	if(!this->privateData)	this->privateData = new PrivateData();

	int result = this->privateData->connection.Reconnect();

	if(result != 0)
	{
		return false;
	}
	this->privateData->owner = 0;
	this->privateData->parent = this;
	this->privateData->thread.Start();

	return true;
}

void NetworkClient::Disconnect()
{
	if(!privateData) return;

	SetEvent(privateData->shutdownEvent);
	privateData->thread.Wait();

	privateData->connection.Disconnect();
	this->privateData->sendQueue.Clear();
	this->privateData->recieveQueue.Clear();
}

void NetworkClient::Send(CustomProtocolObject& protocol)
{
	this->privateData->sendQueue.Push(protocol.GetProtocol());
	WSASetEvent(this->privateData->socketEvents[1]);
}

void NetworkClient::Send(CustomNetProtocol& protocol)
{
	this->privateData->sendQueue.Push(protocol);
	WSASetEvent(this->privateData->socketEvents[1]);
}

void NetworkClient::SetOwner(NetworkSession* owner)
{
	this->privateData->owner = owner;
}

void NetworkClient::SetMessagePump( NetworkClient::ClientEventFunction func )
{
	if( func )
	{
		this->OnRecieve = func;
	}
	else
	{
		this->OnRecieve = OnRecieve_Default;
	}
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
	else
	{
		this->OnRecieve( e );
	}
}

std::string NetworkClient::GetIpAddress()
{
	return this->privateData->connection.GetIpAddress();
}
void NetworkClient::OutputEventData(bool output)
{
	this->privateData->outputEvent;
}