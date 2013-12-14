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
struct ClientDataContainer
{
	Connection connection;

	SmartPointer<IPostBox<CustomNetProtocol>> sendPostBox;
	
	RecieverObject recvObj;
	NetworkProtocolCallbackType callbackType;

	Oyster::Thread::OysterThread thread;
	std::mutex recvObjMutex;
	std::mutex postBoxMutex;

	Translator translator;

	//ID
	static unsigned int currID;
	const unsigned int ID;

	ClientDataContainer(IThreadObject* o)
		: ID(currID++)
	{
		InitWinSock();
		callbackType = NetworkProtocolCallbackType_Unknown;
		sendPostBox = new PostBox<CustomNetProtocol>();
		connection.SetBlockingMode(false);
	}
	ClientDataContainer(IThreadObject* o, unsigned int socket )
		:connection(socket), ID(currID++)
	{
		InitWinSock();
		callbackType = NetworkProtocolCallbackType_Unknown;
		sendPostBox = new PostBox<CustomNetProtocol>();
		connection.SetBlockingMode(false);
	}
	~ClientDataContainer()
	{
		thread.Stop();
		thread.Wait();
		connection.Disconnect();
		callbackType = NetworkProtocolCallbackType_Unknown;

		ShutdownWinSock();
	}
	
};
unsigned int ClientDataContainer::currID = 0;

struct NetworkClient::PrivateData : public IThreadObject
{
	Utility::DynamicMemory::SmartPointer<ClientDataContainer> data;

	PrivateData() { this->data = new ClientDataContainer(this); }
	PrivateData(unsigned int socket) { this->data = new ClientDataContainer(this, socket); }
	~PrivateData() { }

	bool DoWork()
	{
		if(!this->data)return true;
		if(!this->data->connection.IsConnected()) return true;

		Send();
		Recv();

		return true;
	}

	void Send(CustomNetProtocol* protocol)
	{
		if(!data) return;

		this->data->postBoxMutex.lock();
		this->data->sendPostBox->PostMessage(*protocol);
		this->data->postBoxMutex.unlock();
	}

	int Send()
	{
		int errorCode = 0;
		if(!data) return -1;

		this->data->postBoxMutex.lock();
		if(this->data->sendPostBox->IsFull())
		{
			SmartPointer<OysterByte> temp = new OysterByte();
			this->data->translator.Pack(temp, this->data->sendPostBox->FetchMessage());
			errorCode = this->data->connection.Send(temp);
		}
		this->data->postBoxMutex.unlock();

		return errorCode;
	}

	int Recv()
	{
		int errorCode = -1;

		if(this->data->callbackType == NetworkProtocolCallbackType_Function)
		{
			OysterByte temp = OysterByte();
			errorCode = this->data->connection.Recieve(temp);

			if(errorCode == 0)
			{
				CustomNetProtocol protocol;
				bool ok = this->data->translator.Unpack(protocol, temp);
		
				//Check if the protocol was unpacked correctly
				if(ok)
				{
					this->data->recvObjMutex.lock();
					if(this->data->callbackType == NetworkProtocolCallbackType_Function)
					{
						this->data->recvObj.protocolRecieverFnc(protocol);
					}
					else if(this->data->callbackType == NetworkProtocolCallbackType_Object)
					{
						this->data->recvObj.protocolRecievedObject->ProtocolRecievedCallback(protocol);
					}
					this->data->recvObjMutex.unlock();
				}
			}
		}
		return errorCode;
	}

};


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
	this->privateData->data->recvObj = SmartPointer<RecieverObject>(&recvObj);;
}

NetworkClient::NetworkClient(RecieverObject recvObj, NetworkProtocolCallbackType type, unsigned int socket)
{
	privateData = new PrivateData(socket);
	this->privateData->data->recvObj = SmartPointer<RecieverObject>(&recvObj);
	this->privateData->data->callbackType = type;
}

NetworkClient::NetworkClient(const NetworkClient& obj)
{
	this->privateData = new PrivateData(*obj.privateData);
}

NetworkClient& NetworkClient::operator =(const NetworkClient& obj)
{
	delete privateData;
	this->privateData = new PrivateData(*obj.privateData);
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
	int result = this->privateData->data->connection.Connect(port, serverIP);
	
	//Connect has succeeded
	if(result == 0)
	{
		privateData->data->thread.Start();
		return true;
	}

	privateData->data->connection.SetBlockingMode(false);

	//Connect has failed
	return false;
}

void NetworkClient::Disconnect()
{
	privateData->data->connection.Disconnect();
}

bool NetworkClient::IsConnected()
{
	return privateData->data->connection.IsConnected();
}

void NetworkClient::Send(CustomProtocolObject& protocol)
{
	this->privateData->Send(protocol.GetProtocol());
}

void NetworkClient::SetRecieverObject(RecieverObject recvObj, NetworkProtocolCallbackType type)
{
	if (type == NetworkProtocolCallbackType_Unknown) return;

	privateData->data->recvObjMutex.lock();
		privateData->data->recvObj = recvObj;
		privateData->data->callbackType = type;
	privateData->data->recvObjMutex.unlock();
}

bool NetworkClient::operator ==(const NetworkClient& obj)
{
	return (this->privateData->data->ID == obj.privateData->data->ID);
}