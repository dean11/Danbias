
#include "ClientObject.h"
#include "NetworkSession.h"
#include <mutex>

static std::mutex ClientListLock;

namespace DanBias
{
	NetworkSession::NetworkSession()
	{ }
	NetworkSession::NetworkSession(const NetworkSession& orig)
	{
		this->clients = orig.clients;
	}
	const NetworkSession& NetworkSession::operator=(const NetworkSession& orig)
	{
		this->clients = orig.clients;
		return *this;
	}
	NetworkSession::~NetworkSession()
	{
		this->clients.Clear();
	}

	void NetworkSession::AttachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client, Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box)
	{
		ClientListLock.lock();

			int k = -1;
			for (unsigned int i = 0; (k == -1) && i < this->clients.Size(); i++)
			{
				if(!this->clients[i])
					k = i;
			}

			if(k == -1)
			{
				this->clients.Push(client);
				this->clients[this->clients.Size() - 1]->SetPostbox(box);
			}
			else
			{
				this->clients[k] = client;
				this->clients[k]->SetPostbox(box);
			}
		
		ClientListLock.unlock();
		
	}

	Utility::DynamicMemory::SmartPointer<ClientObject> NetworkSession::DetachClient(Oyster::Network::NetworkClient* client)
	{
		Utility::DynamicMemory::SmartPointer<ClientObject> val;

		ClientListLock.lock();

			for (unsigned int i = 0; i < this->clients.Size(); i++)
			{
				if(this->clients[0]->NetClient_Object()->Id() == client->Id())
				{
					val = this->clients[i];
					this->clients[i] = 0;
				}
			}

		ClientListLock.unlock();

		return val;
	}
	Utility::DynamicMemory::SmartPointer<ClientObject> NetworkSession::DetachClient(ClientObject* client)
	{
		Utility::DynamicMemory::SmartPointer<ClientObject> val;

		ClientListLock.lock();

			for (unsigned int i = 0; i < this->clients.Size(); i++)
			{
				if(this->clients[0]->NetClient_Object()->Id() == client->NetClient_Object()->Id())
				{
					val = this->clients[i];
					this->clients[i] = 0;
				}
			}
		
		ClientListLock.unlock();

		return val;
	}
	Utility::DynamicMemory::SmartPointer<ClientObject> NetworkSession::DetachClient(short ID)
	{
		Utility::DynamicMemory::SmartPointer<ClientObject> val;

		ClientListLock.lock();

			for (unsigned int i = 0; i < this->clients.Size(); i++)
			{
				if(this->clients[0]->NetClient_Object()->Id() == ID)
				{
					val = this->clients[i];
					this->clients[i] = 0;
				}
			}

		ClientListLock.unlock();

		return val;
	}
	
	void NetworkSession::Send(Oyster::Network::CustomNetProtocol& protocol)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			this->clients[i]->NetClient_Object()->Send(&protocol);
		}
	}
	void NetworkSession::Send(Oyster::Network::CustomNetProtocol& protocol, int ID)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i]->NetClient_Object()->Id() == ID)
			{
				this->clients[i]->NetClient_Object()->Send(&protocol);
				break;
			}
		}
	}

	void NetworkSession::SetPostbox(Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			this->clients[i]->SetPostbox(box);
		}
	}

	void NetworkSession::CloseSession(NetworkSession * owner)
	{
		ClientListLock.lock();

			if(!owner)
			{
				for (unsigned int i = 0; i < this->clients.Size(); i++)
				{
					this->clients[i]->NetClient_Object()->Disconnect();
				}
			}
			else
			{
				for (unsigned int i = 0; i < this->clients.Size(); i++)
				{
					owner->AttachClient(this->clients[i]);
				}
			}

			this->clients.Clear();

		ClientListLock.unlock();
	}

}//End namespace DanBias