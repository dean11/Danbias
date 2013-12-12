#ifndef DANBIASSERVER_NETWORK_SESSION_H
#define DANBIASSERVER_NETWORK_SESSION_H

#include "Utilities.h"
#include "ClientObject.h"
#include <PlayerProtocols.h>
#include <vector>

namespace DanBias
{
	class NetworkSession
	{
	public:
		NetworkSession();
		~NetworkSession();

		void AttachClient(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client);
		
		void DetachClient(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client);
		void DetachClient(short ID);
		
		void Kick(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client);
		void Kick();

		void Send(Oyster::Network::CustomNetProtocol& protocol);
		void Send(Oyster::Network::CustomNetProtocol& protocol, int ID);

		//TODO: Do more lobby features

	protected:
		Oyster::Network::NetworkClient* operator[](int Identification);

	private:
		std::vector<Utility::DynamicMemory::SmartPointer<ClientObject>> clients;
	};
}//End namespace DanBias
#endif // !DANBIASSERVER_NETWORK_SESSION_H
