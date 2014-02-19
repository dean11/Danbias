/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef NETWORK_API_NETWORK_SESSION_H
#define NETWORK_API_NETWORK_SESSION_H

//warning C4150: deletion of pointer to incomplete type, no destructor called
#pragma warning(disable : 4150)
//needs to have dll-interface to be used by clients of class 'Oyster::Network::NetworkSession'
#pragma warning(disable : 4251)


#include "NetworkAPI_Preprocessor.h"
#include "NetworkServerEventStruct.h"
#include "NetworkClient.h"
#include "Utilities.h"
#include "DynamicArray.h"

namespace Oyster
{
	namespace Network
	{
		typedef Utility::DynamicMemory::SmartPointer<NetworkClient> NetClient;
		typedef Utility::DynamicMemory::DynamicArray<NetClient>	NetClientList;
		class NET_API_EXPORT NetworkSession
		{
		public:
			NetworkSession();
			NetworkSession(const NetworkSession& orig);
			const NetworkSession& operator=(const NetworkSession& orig);
			virtual~NetworkSession();

			/**	Parse session events such as protocols recieved etc.
			*/
			virtual void ProcessClients();

			/**
			*	
			*/
			virtual bool Attach(NetClient client);
		
			/**
			*	Detaches all clients and sends them to owner.
			*	If no owner is set the clients is disconnected.
			*/
			virtual void DetachAll();

			/**
			*	
			*/
			virtual NetClient Detach(const NetworkClient* client);
	
			/**
			*	
			*/
			virtual NetClient Detach(short ID);

			/**
			*	
			*/
			virtual NetClient Detach();

			/**	Send a message to all clients in this session
			*	@param message The message
			*/
			virtual bool Send(Oyster::Network::CustomNetProtocol& message);

			/**	Send a message to a specific client in this session
			*	@param message The message
			*/
			virtual bool Send(Oyster::Network::CustomNetProtocol& protocol, int ID);

			/**
			*	
			*/
			virtual void CloseSession( bool dissconnectClients = false ); 

			/** 
			*	Set the owner that clients will be returned to.
			*	@param owner If owner is NULL, clients will be disconnected when session is over.
			*/
			virtual void SetOwner(NetworkSession* owner);

			/**	Get the number of clients active in this session
			*	@return The client count
			*/
			inline int GetClientCount() const { return this->clientCount; }

			/**
			*	
			*/
			virtual void ClientConnectedEvent(NetClient client);

			/**
			*	
			*/
			virtual void ClientEventCallback(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e) = 0;

		protected:
			NetClientList clients;
			int clientCount;
			
		private:
			struct PrivateSessionData;
			PrivateSessionData* data;
		};
	}
}//End namespace DanBias

#endif // !DANBIASSERVER_NETWORK_SESSION_H
