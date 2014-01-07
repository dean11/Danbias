/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_NETWORK_SESSION_H
#define DANBIASSERVER_NETWORK_SESSION_H

//warning C4150: deletion of pointer to incomplete type, no destructor called
#pragma warning(disable : 4150)

#define NOMINMAX

#include "INetworkSession.h"
#include <Utilities.h>
#include <OysterCallback.h>
#include <DynamicArray.h>
#include <PostBox\IPostBox.h>
#include <CustomNetProtocol.h>
#include <NetworkClient.h>
#include <vector>
#include <mutex>

namespace DanBias
{
	class LobbyClient;
	class GameClient;

	class NetworkSession	:public INetworkSession
	{
	public:
		struct NetEvent
		{
			LobbyClient* sender;
			GameClient*	gameClient;
			Oyster::Network::CustomNetProtocol protocol;
			NetEvent():sender(0), gameClient(0){}
		};

	public:
		NetworkSession();
		NetworkSession(const NetworkSession& orig);
		const NetworkSession& operator=(const NetworkSession& orig);
		virtual~NetworkSession();

		virtual bool Attach(Utility::DynamicMemory::SmartPointer<LobbyClient> client);
		
		virtual Utility::DynamicMemory::SmartPointer<LobbyClient> Detach(Oyster::Network::NetworkClient* client);
		virtual Utility::DynamicMemory::SmartPointer<LobbyClient> Detach(const LobbyClient* client);
		virtual Utility::DynamicMemory::SmartPointer<LobbyClient> Detach(const LobbyClient& client);
		virtual Utility::DynamicMemory::SmartPointer<LobbyClient> Detach(short ID);

		Utility::DynamicMemory::SmartPointer<LobbyClient> FindClient(LobbyClient& obj);
		Utility::DynamicMemory::SmartPointer<LobbyClient> FindClient(LobbyClient* obj);
		Utility::DynamicMemory::SmartPointer<LobbyClient> FindClient(int ID);
		
		/**
		*	Sends a message to all clients in this session.
		*/
		virtual bool Send(Oyster::Network::CustomNetProtocol& message);
		/**
		*	Sends a message to a specific client in this session.
		*/
		virtual bool Send(Oyster::Network::CustomNetProtocol& protocol, int ID);

		/**
		*	Set the callback to all clients to where a messages is recieved.
		*/
		virtual void SetCallback(Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value);

		/**
		*	Closes the session and sends the clients to given owner session if any. 
		*	If session is null, clients is assumed to already be elsewhere and only releases a reference.
		*/
		virtual void CloseSession(bool dissconnectClients = false); 

		
		/** Set where the clients is returned on closed session. */
		inline void SetOwner(NetworkSession* owner) { this->owner = owner; }

	protected:
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<LobbyClient>> clients;
		NetworkSession* owner; //Where clients end up when session is closed.

	private:
		std::mutex clientListLock;
		int clientCount;

	private:
		friend class AdminInterface;
	};
}//End namespace DanBias
#endif // !DANBIASSERVER_NETWORK_SESSION_H
