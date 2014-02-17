/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "NetworkSession.h"
#include "Utilities.h"
#include "DynamicArray.h"
#include "GID.h"
#include "CustomNetProtocol.h"
#include <vector>
#include <mutex>

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;


struct NetworkSession::PrivateSessionData
{
	NetworkClient::ClientEventFunction messageCallback;
	std::mutex clientListLock;
	NetworkSession* owner; //Where clients end up when session is closed.
	int id;
	NetworkSession::PrivateSessionData()
		:	owner(0)
		,	id(GID())
	{}
};

int FindClient(NetClientList& list, NetClient c)
{
	for (unsigned int i = 0; i < list.Size(); i++)
	{
		if(c == list[i])
			return i;
	}
	return -1;
}

NetworkSession::NetworkSession()
	:	data(new PrivateSessionData())
{}
NetworkSession::NetworkSession(const NetworkSession& orig)
{
	this->clients = orig.clients;
	this->data->owner = orig.data->owner;
	this->clientCount = orig.clientCount;
	this->data->id = orig.data->id;
	this->data->messageCallback = orig.data->messageCallback;
}

const NetworkSession& NetworkSession::operator=(const NetworkSession& orig)
{
	this->clients = orig.clients;
	this->data->owner = orig.data->owner;
	this->clientCount = orig.clientCount;
	this->data->id = orig.data->id;
	this->data->messageCallback = orig.data->messageCallback;
		
	return *this;
}

NetworkSession::~NetworkSession()
{
	this->clients.Clear();
	this->clientCount = 0;
	this->data->messageCallback = 0;
	delete this->data;
	this->data = 0;
}

void NetworkSession::ProcessClients()
{
	for (unsigned int i = 0; i < this->clients.Size(); i++)
	{
		if(this->clients[i])	this->clients[i]->Update();
	}
}

bool NetworkSession::Attach(NetClient client)
{
	this->data->clientListLock.lock();

		int k = -1;
		for (unsigned int i = 0; (k == -1) && i < this->clients.Size(); i++)
		{
			if(!this->clients[i]->IsConnected())	//TODO: Dont check connection status, check more general status..
				k = i;
		}

		if(k == -1)
		{
			this->clients.Push(client);
		}
		else
		{
			this->clients[k] = client;
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
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			this->data->owner->Attach(this->clients[i]);
			this->clients[i] = 0;
		}
	}
	else
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			this->clients[i]->Disconnect();
			this->clients[i] = 0;
		}
	}
	this->clientCount = 0;
}

NetClient NetworkSession::Detach(const NetworkClient* client)
{
	NetClient val;

	this->data->clientListLock.lock();

		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i] && this->clients[i]->GetID() == client->GetID())
			{
				val = this->clients[i];
				this->clients[i] = 0;
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

		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i] && this->clients[0]->GetID() == ID)
			{
				val = this->clients[i];
				this->clients[i] = 0;
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

		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i])
			{
				val = this->clients[i];
				this->clients[i] = 0;
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
	for (unsigned int i = 0; i < this->clients.Size(); i++)
	{
		if(this->clients[i])
		{
			this->clients[i]->Send(protocol);
			returnValue = true;
		}
	}

	return returnValue;
}

bool NetworkSession::Send(Oyster::Network::CustomNetProtocol& protocol, int ID)
{
	for (unsigned int i = 0; i < this->clients.Size(); i++)
	{
		if(this->clients[i] && this->clients[i]->GetID() == ID)
		{
			this->clients[i]->Send(protocol);
			return true;
		}
	}
	return false;
}

void NetworkSession::CloseSession(bool dissconnectClients)
{
	this->data->clientListLock.lock();

		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i])
			{
				if(dissconnectClients)		this->clients[i]->Disconnect();
				else if(this->data->owner)	this->data->owner->Attach(this->clients[i]);
				else						this->clients[i]->Disconnect();				//Idiot check, clients have to go somewhere..
			}
		}

		this->clients.Clear();
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

	if(FindClient(this->clients, client) == -1)
		NetworkSession::Attach(client);
}

