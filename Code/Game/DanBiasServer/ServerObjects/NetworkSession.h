/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_NETWORK_SESSION_H
#define DANBIASSERVER_NETWORK_SESSION_H

#define NOMINMAX
#include "Utilities.h"
#include <PostBox\IPostBox.h>
#include <CustomNetProtocol.h>
#include <NetworkClient.h>
#include <vector>

namespace DanBias
{
	class ClientObject;
	class NetworkSession
	{
	public:
		struct NetEvent
		{
			ClientObject* reciever;
			Oyster::Network::CustomNetProtocol protocol;
		};

	public:
		NetworkSession();
		~NetworkSession();

		void AttachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client);
		
		void DetachClient(Oyster::Network::NetworkClient* client);
		void DetachClient(ClientObject* client);
		void DetachClient(short ID);
		void DetachClient();
		
		void Kick();

		void Send(Oyster::Network::CustomNetProtocol& protocol);
		void Send(Oyster::Network::CustomNetProtocol& protocol, int ID);

		//TODO: Do more lobby features
		void SetPostbox(Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box);

	protected:
		std::vector<Utility::DynamicMemory::SmartPointer<ClientObject>> clients;
		Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box;
	};
}//End namespace DanBias
#endif // !DANBIASSERVER_NETWORK_SESSION_H
