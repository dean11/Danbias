#ifndef DANBIASSERVER_NETWORK_SESSION_H
#define DANBIASSERVER_NETWORK_SESSION_H

#include "Utilities.h"
#include <PostBox\PostBox.h>
#include <PlayerProtocols.h>
#include <vector>

namespace DanBias
{
	class ClientObject;
	class NetworkSession
	{
	public:
		struct ClientEvent
		{
			ClientObject* reciever;
			Oyster::Network::CustomNetProtocol protocol;
			ClientEvent()  { reciever = 0; }
			~ClientEvent() { }
		};

	public:
		NetworkSession();
		~NetworkSession();

		void AttachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client);
		
		void DetachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client);
		void DetachClient(short ID);
		
		void Kick(Utility::DynamicMemory::SmartPointer<ClientObject> client);
		void Kick();

		void Send(Oyster::Network::CustomNetProtocol& protocol);
		void Send(Oyster::Network::CustomNetProtocol& protocol, int ID);

		//TODO: Do more lobby features

	protected:
		std::vector<Utility::DynamicMemory::SmartPointer<ClientObject>> clients;
		Oyster::PostBox<DanBias::NetworkSession::ClientEvent> box;
	};
}//End namespace DanBias
#endif // !DANBIASSERVER_NETWORK_SESSION_H
