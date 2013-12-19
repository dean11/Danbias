/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_NETWORK_SESSION_H
#define DANBIASSERVER_NETWORK_SESSION_H

#pragma warning(disable: 4150)

#define NOMINMAX
#include <Utilities.h>
#include <DynamicArray.h>
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
		NetworkSession(const NetworkSession& orig);
		const NetworkSession& operator=(const NetworkSession& orig);
		virtual~NetworkSession();

		virtual void AttachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client, Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box = 0);
		
		virtual Utility::DynamicMemory::SmartPointer<ClientObject> DetachClient(Oyster::Network::NetworkClient* client);
		virtual Utility::DynamicMemory::SmartPointer<ClientObject> DetachClient(ClientObject* client);
		virtual Utility::DynamicMemory::SmartPointer<ClientObject> DetachClient(short ID);
		
		virtual void Send(Oyster::Network::CustomNetProtocol& protocol);
		virtual void Send(Oyster::Network::CustomNetProtocol& protocol, int ID);

		//TODO: Do more lobby features
		virtual void SetPostbox(Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box);

		virtual void CloseSession(NetworkSession* clientDestination); //<! Closes the session and sends the clients to given sesison. If session is null, clients is kicked from server.

		Utility::DynamicMemory::SmartPointer<ClientObject> FindClient(int ID);
		Utility::DynamicMemory::SmartPointer<ClientObject> FindClient(ClientObject& obj);

	protected:
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<ClientObject>> clients;
	};
}//End namespace DanBias
#endif // !DANBIASSERVER_NETWORK_SESSION_H
