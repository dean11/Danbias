
#include "ClientObject.h"
#include "NetworkSession.h"
#include <mutex>

static std::mutex ClientListLock;

namespace DanBias
{
	NetworkSession::NetworkSession()
	{

	}
	NetworkSession::~NetworkSession()
	{

	}

	void NetworkSession::AttachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client)
	{
		ClientListLock.lock();

			int k = -1;
			for (unsigned int i = 0; (k == -1) && i < this->clients.size(); i++)
			{
				if(!this->clients[i])
					k = i;
			}

			if(k == -1)
			{
				this->clients.push_back(client);
				this->clients[this->clients.size() - 1]->SetPostbox(this->box);
			}
			else
			{
				this->clients[k]->SetPostbox(this->box);
			}
		
		ClientListLock.unlock();
		
	}

	void NetworkSession::DetachClient(Oyster::Network::NetworkClient* client)
	{
		ClientListLock.lock();

			for (unsigned int i = 0; i < this->clients.size(); i++)
			{
				if(this->clients[0]->NetClient_Object()->Id() == client->Id())
					this->clients[i] = 0;
			}

		ClientListLock.unlock();
	}
	void NetworkSession::DetachClient(ClientObject* client)
	{
		ClientListLock.lock();

			for (unsigned int i = 0; i < this->clients.size(); i++)
			{
				if(this->clients[0]->NetClient_Object()->Id() == client->NetClient_Object()->Id())
					this->clients[i] = 0;
			}
		
		ClientListLock.unlock();
	}
	void NetworkSession::DetachClient(short ID)
	{
		ClientListLock.lock();

			for (unsigned int i = 0; i < this->clients.size(); i++)
			{
				if(this->clients[0]->NetClient_Object()->Id() == ID)
					this->clients[i] = 0;
			}

		ClientListLock.unlock();
	}
	void NetworkSession::DetachClient()
	{
		ClientListLock.lock();

			for (unsigned int i = 0; i < this->clients.size(); i++)
			{
				this->clients[i] = 0;
			}

		ClientListLock.unlock();
	}
	
	void NetworkSession::Kick()
	{
		ClientListLock.lock();

			for (unsigned int i = 0; i < this->clients.size(); i++)
			{
				this->clients[i]->NetClient_Object()->Disconnect();
				this->clients[i] = 0;
			}

		ClientListLock.unlock();
	}

	void NetworkSession::SetPostbox(Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box)
	{
		this->box = box;
	}
}//End namespace DanBias