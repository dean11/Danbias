#ifndef INCLUDE_WINSOCK_LIB
#define INCLUDE_WINSOCK_LIB
	#pragma comment(lib, "ws2_32.lib")
#endif

#include "NetworkClient.h"

#include "Translator.h"
#include "CustomNetProtocol.h"

#include "../NetworkDependencies/Connection.h"
#include "../NetworkDependencies/PostBox.h"
#include "../NetworkDependencies/WinsockFunctions.h"

#include "../../Misc/Utilities.h"
#include "../../Misc/Thread/IThreadObject.h"
#include "../../Misc/Thread/OysterThread.h"

using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace Utility::DynamicMemory;

/*************************************
			PrivateData
*************************************/

struct NetworkClient::PrivateData : public IThreadObject
{
	PrivateData();
	PrivateData(unsigned int socket);
	~PrivateData();

	void Start();

	void Send(CustomNetProtocol& protocol);	//Is called from the outside to add messages to send.

	//Called on from the thread
	int Send();
	int Recv();

	bool DoWork();

	SmartPointer<Connection> connection;

	SmartPointer<IPostBox<CustomNetProtocol>> sendPostBox;
	
	SmartPointer<RecieverObject> recvObj;
	NetworkProtocolCallbackType callbackType;

	Oyster::Thread::OysterThread thread;
	std::mutex recvObjMutex;
	std::mutex postBoxMutex;

	Translator translator;

	//ID
	static unsigned int currID;
	const unsigned int ID;
};

unsigned int NetworkClient::PrivateData::currID = 0;

NetworkClient::PrivateData::PrivateData() : ID(currID++)
{
	InitWinSock();

	callbackType = NetworkProtocolCallbackType_Unknown;

	connection = new Connection();
	sendPostBox = new PostBox<CustomNetProtocol>;
	this->thread.Create(this, false);

	Start();
}

NetworkClient::PrivateData::PrivateData(unsigned int socket) : ID(currID++)
{
	InitWinSock();

	callbackType = NetworkProtocolCallbackType_Unknown;

	connection = new Connection(socket);
	sendPostBox = new PostBox<CustomNetProtocol>;
	this->thread.Create(this, false);

	connection->SetBlockingMode(false);

	Start();
}

NetworkClient::PrivateData::~PrivateData()
{
	thread.Stop();

	/*if(connection)
	{
		delete connection;
		connection = NULL;
	}*/

	/*if(sendPostBox)
	{
		delete sendPostBox;
		sendPostBox = NULL;
	}*/

	callbackType = NetworkProtocolCallbackType_Unknown;

	ShutdownWinSock();
}

bool NetworkClient::PrivateData::DoWork()
{
	Send();
	Recv();

	return true;
}

void NetworkClient::PrivateData::Send(CustomNetProtocol& protocol)
{
	postBoxMutex.lock();
	sendPostBox->PostMessage(protocol);
	postBoxMutex.unlock();
}

int NetworkClient::PrivateData::Send()
{
	int errorCode = 0;

	postBoxMutex.lock();
	if(sendPostBox->IsFull())
	{
		SmartPointer<OysterByte> temp = new OysterByte;
		this->translator.Pack(temp, sendPostBox->FetchMessage());
		errorCode = this->connection->Send(temp);
	}
	postBoxMutex.unlock();

	return errorCode;
}

int NetworkClient::PrivateData::Recv()
{
	int errorCode = -1;
	
	SmartPointer<OysterByte> temp = new OysterByte;
	errorCode = this->connection->Recieve(temp);

	if(errorCode == 0)
	{
		CustomNetProtocol protocol;
		bool ok = translator.Unpack(protocol, temp);
		
		//Check if the protocol was unpacked correctly
		if(ok)
		{
			recvObjMutex.lock();
			if(callbackType == NetworkProtocolCallbackType_Function)
			{
				recvObj->protocolRecieverFnc(protocol);
			}
			else if(callbackType == NetworkProtocolCallbackType_Object)
			{
				recvObj->protocolRecievedObject->ProtocolRecievedCallback(protocol);
			}
			recvObjMutex.unlock();
		}
	}

	return errorCode;
}

void NetworkClient::PrivateData::Start()
{
	this->thread.Start();
}

/*************************************
			NetworkClient
*************************************/

NetworkClient::NetworkClient()
{
	privateData = new PrivateData();
}

NetworkClient::NetworkClient(unsigned int socket)
{
	privateData = new PrivateData(socket);
}

NetworkClient::NetworkClient(RecieverObject recvObj, NetworkProtocolCallbackType type)
{
	privateData = new PrivateData();
	this->privateData->recvObj = SmartPointer<RecieverObject>(&recvObj);;
}

NetworkClient::NetworkClient(RecieverObject recvObj, NetworkProtocolCallbackType type, unsigned int socket)
{
	privateData = new PrivateData(socket);
	this->privateData->recvObj = SmartPointer<RecieverObject>(&recvObj);
	this->privateData->callbackType = type;
}

NetworkClient::NetworkClient(const NetworkClient& obj)
{
	this->privateData = new PrivateData();

	this->privateData = obj.privateData;
}

NetworkClient& NetworkClient::operator =(const NetworkClient& obj)
{
	delete privateData;
	this->privateData = new PrivateData();
	this->privateData = obj.privateData;
	return *this;
}

NetworkClient::~NetworkClient()
{
	if(privateData)
	{
		delete privateData;
		privateData = NULL;
	}
}

bool NetworkClient::Connect(unsigned short port, const char serverIP[])
{
	int result = this->privateData->connection->Connect(port, serverIP);
	
	//Connect has succeeded
	if(result == 0)
	{
		privateData->Start();
		return true;
	}

	privateData->connection->SetBlockingMode(false);

	//Connect has failed
	return false;
}

void NetworkClient::Disconnect()
{
	privateData->connection->Disconnect();
}

bool NetworkClient::IsConnected()
{
	return privateData->connection->IsConnected();
}

void NetworkClient::Send(CustomProtocolObject& protocol)
{
	this->privateData->Send(*protocol.GetProtocol());
}

void NetworkClient::SetRecieverObject(RecieverObject recvObj, NetworkProtocolCallbackType type)
{
	privateData->recvObjMutex.lock();
	privateData->recvObj = SmartPointer<RecieverObject>(&recvObj);
	privateData->callbackType = type;
	privateData->recvObjMutex.unlock();
}

bool NetworkClient::operator ==(const NetworkClient& obj)
{
	return (this->privateData->ID == obj.privateData->ID);
}