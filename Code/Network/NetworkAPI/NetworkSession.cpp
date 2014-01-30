/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "NetworkSession.h"
#include "..\..\Misc\Utilities.h"
#include "..\..\Misc\DynamicArray.h"
#include "..\..\Misc\GID.h"
#include "CustomNetProtocol.h"
#include <vector>
#include <mutex>

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;


struct NetworkSession::PrivateSessionData
{
	Utility::DynamicMemory::DynamicArray<NetClient> clients;
	NetworkClient::ClientEventFunction messageCallback;
	std::mutex clientListLock;
	NetworkSession* owner; //Where clients end up when session is closed.
	int id;
	NetworkSession::PrivateSessionData()
		:	owner(0)
		,	id(GID())
	{}
};



NetworkSession::NetworkSession()
	:	data(new PrivateSessionData())
{}
NetworkSession::NetworkSession(const NetworkSession& orig)
{
	this->data->clients = orig.data->clients;
	this->data->owner = orig.data->owner;
	this->clientCount = orig.clientCount;
	this->data->id = orig.data->id;
	this->data->messageCallback = orig.data->messageCallback;
}

const NetworkSession& NetworkSession::operator=(const NetworkSession& orig)
{
	this->data->clients = orig.data->clients;
	this->data->owner = orig.data->owner;
	this->clientCount = orig.clientCount;
	this->data->id = orig.data->id;
	this->data->messageCallback = orig.data->messageCallback;
		
	return *this;
}

NetworkSession::~NetworkSession()
{
	this->data->clients.Clear();
	this->clientCount = 0;
	this->data->messageCallback = 0;
	delete this->data;
	this->data = 0;
}

void NetworkSession::ProcessClients()
{
	for (unsigned int i = 0; i < this->data->clients.Size(); i++)
	{
		if(this->data->clients[i])	this->data->clients[i]->Update();
	}
}

bool NetworkSession::Attach(NetClient client)
{
	this->data->clientListLock.lock();

		int k = -1;
		for (unsigned int i = 0; (k == -1) && i < this->data->clients.Size(); i++)
		{
			if(!this->data->clients[i]->IsConnected())	//TODO: Dont check connection status, check more general status..
				k = i;
		}

		if(k == -1)
		{
			this->data->clients.Push(client);
		}
		else
		{
			this->data->clients[k] = client;
		}

		this->clientCount++;

	client->SetOwner(this);
	this->data->clientListLock.unlock();

	return true;
}

void NetworkSession::DetachAll()
{
	if(this->data->owner)
	{
		for (unsigned int i = 0; i < this->data->clients.Size(); i++)
		{
			this->data->owner->Attach(this->data->clients[i]);
			this->data->clients[i] = 0;
		}
	}
	else
	{
		for (unsigned int i = 0; i < this->data->clients.Size(); i++)
		{
			this->data->clients[i]->Disconnect();
			this->data->clients[i] = 0;
		}
	}
	this->clientCount = 0;
}

NetClient NetworkSession::Detach(const NetworkClient* client)
{
	NetClient val;

	this->data->clientListLock.lock();

		for (unsigned int i = 0; i < this->data->clients.Size(); i++)
		{
			if(this->data->clients[i] && this->data->clients[0]->GetID() == client->GetID())
			{
				val = this->data->clients[i];
				this->data->clients[i] = 0;
				this->clientCount--;
			}
		}
		
	this->data->clientListLock.unlock();

	return val;
}

NetClient NetworkSession::Detach(short ID)
{
	NetClient val;

	this->data->clientListLock.lock();

		for (unsigned int i = 0; i < this->data->clients.Size(); i++)
		{
			if(this->data->clients[i] && this->data->clients[0]->GetID() == ID)
			{
				val = this->data->clients[i];
				this->data->clients[i] = 0;
				this->clientCount--;
			}
		}

	this->data->clientListLock.unlock();

	return val;
}

NetClient NetworkSession::Detach()
{
	NetClient val;

	this->data->clientListLock.lock();

		for (unsigned int i = 0; i < this->data->clients.Size(); i++)
		{
			if(this->data->clients[i])
			{
				val = this->data->clients[i];
				this->data->clients[i] = 0;
				this->clientCount--;
				break;
			}
		}

	this->data->clientListLock.unlock();

	return val;
}
	
bool NetworkSession::Send(Oyster::Network::CustomNetProtocol& protocol)
{
	bool returnValue = false;
	for (unsigned int i = 0; i < this->data->clients.Size(); i++)
	{
		if(this->data->clients[i])
		{
			this->data->clients[i]->Send(&protocol);
			returnValue = true;
		}
	}

	return returnValue;
}

bool NetworkSession::Send(Oyster::Network::CustomNetProtocol& protocol, int ID)
{
	for (unsigned int i = 0; i < this->data->clients.Size(); i++)
	{
		if(this->data->clients[i] && this->data->clients[i]->GetID() == ID)
		{
			this->data->clients[i]->Send(&protocol);
			return true;
		}
	}
	return false;
}

void NetworkSession::CloseSession(bool dissconnectClients)
{
	this->data->clientListLock.lock();

		for (unsigned int i = 0; i < this->data->clients.Size(); i++)
		{
			if(this->data->clients[i])
			{
				if(dissconnectClients)		this->data->clients[i]->Disconnect();
				else if(this->data->owner)	this->data->owner->Attach(this->data->clients[i]);
				else						this->data->clients[i]->Disconnect();				//Idiot check, clients have to go somewhere..
			}
		}

		this->data->clients.Clear();
		this->clientCount = 0;

	this->data->clientListLock.unlock();
}

void NetworkSession::SetOwner(NetworkSession* owner)
{
	this->data->owner = owner;
}

void NetworkSession::ClientConnectedEvent(NetClient client)
{
	this->Attach(client);
}

