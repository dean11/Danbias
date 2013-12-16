
#include "ClientObject.h"
#include "NetworkSession.h"
#include <mutex>

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
		for (unsigned int i = 0; i < this->clients.size(); i++)
		{
			if(!this->clients[i])
			{
				this->clients[i] = client;
				this->clients[i]->SetPostbox(&this->box);
				return;
			}
		}
		this->clients.push_back(client);
	}

	void NetworkSession::DetachClient(Oyster::Network::NetworkClient* client)
	{
		for (unsigned int i = 0; i < this->clients.size(); i++)
		{
			if(this->clients[0]->NetClient_Object()->Id() == client->Id())
				this->clients[i] = 0;
		}
	}
	void NetworkSession::DetachClient(ClientObject* client)
	{
		for (unsigned int i = 0; i < this->clients.size(); i++)
		{
			if(this->clients[0]->NetClient_Object()->Id() == client->NetClient_Object()->Id())
				this->clients[i] = 0;
		}
		
	}
	void NetworkSession::DetachClient(short ID)
	{
		for (unsigned int i = 0; i < this->clients.size(); i++)
		{
			if(this->clients[0]->NetClient_Object()->Id() == ID)
				this->clients[i] = 0;
		}
		
	}
	void NetworkSession::DetachClient()
	{
		for (unsigned int i = 0; i < this->clients.size(); i++)
		{
			this->clients[i] = 0;
		}
	}
	
	void NetworkSession::Kick()
	{
		for (unsigned int i = 0; i < this->clients.size(); i++)
		{
			this->clients[i]->NetClient_Object()->Disconnect();
			this->clients[i] = 0;
		}
	}
}//End namespace DanBias