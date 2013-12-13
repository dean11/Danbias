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
				//this->clients[i]->SetPostbox(&this->box);
				return;
			}
		}
		this->clients.push_back(client);
	}
	void NetworkSession::DetachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client)
	{

	}
	void NetworkSession::DetachClient(short ID)
	{

	}
	
	void NetworkSession::Kick(Utility::DynamicMemory::SmartPointer<ClientObject> client)
	{

	}
	void NetworkSession::Kick()
	{

	}
}//End namespace DanBias