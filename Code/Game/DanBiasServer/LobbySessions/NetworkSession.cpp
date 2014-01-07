/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "LobbyClient.h"
#include "NetworkSession.h"
#include <mutex>



namespace DanBias
{
	NetworkSession::NetworkSession()
		:	owner(0)
		,	clientCount(0)
	{}
	NetworkSession::NetworkSession(const NetworkSession& orig)
	{
		this->clients = orig.clients;
		this->owner = orig.owner;
		this->clientCount = orig.clientCount;
	}
	const NetworkSession& NetworkSession::operator=(const NetworkSession& orig)
	{
		this->clients = orig.clients;
		this->owner = orig.owner;
		this->clientCount = orig.clientCount;
		return *this;
	}
	NetworkSession::~NetworkSession()
	{
		this->clients.Clear();
		this->clientCount = 0;
	}

	bool NetworkSession::Attach(Utility::DynamicMemory::SmartPointer<LobbyClient> client)
	{
		clientListLock.lock();

			int k = -1;
			for (unsigned int i = 0; (k == -1) && i < this->clients.Size(); i++)
			{
				if(!this->clients[i])
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
		clientListLock.unlock();

		return true;
	}

	Utility::DynamicMemory::SmartPointer<LobbyClient> NetworkSession::Detach(Oyster::Network::NetworkClient* client)
	{
		Utility::DynamicMemory::SmartPointer<LobbyClient> val;

		clientListLock.lock();

			for (unsigned int i = 0; i < this->clients.Size(); i++)
			{
				if(this->clients[0]->GetID() == client->GetID())
				{
					val = this->clients[i];
					this->clients[i] = 0;
					this->clientCount--;
				}
			}

		clientListLock.unlock();

		return val;
	}
	Utility::DynamicMemory::SmartPointer<LobbyClient> NetworkSession::Detach(const LobbyClient* client)
	{
		Utility::DynamicMemory::SmartPointer<LobbyClient> val;

		clientListLock.lock();

			for (unsigned int i = 0; i < this->clients.Size(); i++)
			{
				if(this->clients[0]->GetID() == client->GetID())
				{
					val = this->clients[i];
					this->clients[i] = 0;
					this->clientCount--;
				}
			}
		
		clientListLock.unlock();

		return val;
	}
	Utility::DynamicMemory::SmartPointer<LobbyClient> NetworkSession::Detach(const LobbyClient& client)
	{
		Utility::DynamicMemory::SmartPointer<LobbyClient> val;

		clientListLock.lock();

			for (unsigned int i = 0; i < this->clients.Size(); i++)
			{
				if(this->clients[0]->GetID() == client.GetID())
				{
					val = this->clients[i];
					this->clients[i] = 0;
					this->clientCount--;
				}
			}
		
		clientListLock.unlock();

		return val;
	}
	Utility::DynamicMemory::SmartPointer<LobbyClient> NetworkSession::Detach(short ID)
	{
		Utility::DynamicMemory::SmartPointer<LobbyClient> val;

		clientListLock.lock();

			for (unsigned int i = 0; i < this->clients.Size(); i++)
			{
				if(this->clients[0]->GetID() == ID)
				{
					val = this->clients[i];
					this->clients[i] = 0;
					this->clientCount--;
				}
			}

		clientListLock.unlock();

		return val;
	}
	
	bool NetworkSession::Send(Oyster::Network::CustomNetProtocol& protocol)
	{
		bool returnValue = false;
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i])
			{
				this->clients[i]->Send(&protocol);
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
				this->clients[i]->Send(&protocol);
				return true;
			}
		}
		return false;
	}

	void NetworkSession::SetCallback(Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			this->clients[i]->SetCallback(value);
		}
	}

	void NetworkSession::CloseSession(bool dissconnectClients)
	{
		clientListLock.lock();

			for (unsigned int i = 0; i < this->clients.Size(); i++)
			{
				if(dissconnectClients)	this->clients[i]->Disconnect();
				else if(this->owner)	this->owner->Attach(this->clients[i]);
			}

			this->clients.Clear();

		clientListLock.unlock();
	}
	Utility::DynamicMemory::SmartPointer<LobbyClient> NetworkSession::FindClient(int ID)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i]->GetID() == ID)
				return this->clients[i];
		}
		return Utility::DynamicMemory::SmartPointer<LobbyClient>();
	}
	Utility::DynamicMemory::SmartPointer<LobbyClient> NetworkSession::FindClient(LobbyClient& obj)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i]->GetID() == obj.GetID())
				return this->clients[i];
		}
		return Utility::DynamicMemory::SmartPointer<LobbyClient>();
	}
	Utility::DynamicMemory::SmartPointer<LobbyClient> NetworkSession::FindClient(LobbyClient* obj)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i]->GetID() == obj->GetID())
				return this->clients[i];
		}
		return Utility::DynamicMemory::SmartPointer<LobbyClient>();
	}

	 
}//End namespace DanBias