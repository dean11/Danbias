/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef NETWORK_API_NETWORK_SESSION_H
#define NETWORK_API_NETWORK_SESSION_H

//warning C4150: deletion of pointer to incomplete type, no destructor called
#pragma warning(disable : 4150)

#include "NetworkAPI_Preprocessor.h"
#include "NetworkServerEventStruct.h"
#include "NetworkClient.h"

namespace Oyster
{
	namespace Network
	{
		class NET_API_EXPORT NetworkSession
		{
		public:
			NetworkSession();
			NetworkSession(const NetworkSession& orig);
			const NetworkSession& operator=(const NetworkSession& orig);
			virtual~NetworkSession();

			/**	Parse session events such as protocols recieved etc.
			*/
			void ProcessClients();

			/**
			*	
			*/
			bool Attach(NetworkClient client);
		
			/**
			*	
			*/
			NetworkClient Detach(const NetworkClient& client);
	
			/**
			*	
			*/
			NetworkClient Detach(short ID);

			/**	Send a message to all clients in this session
			*	@param message The message
			*/
			bool Send(Oyster::Network::CustomNetProtocol& message);

			/**	Send a message to a specific client in this session
			*	@param message The message
			*/
			bool Send(Oyster::Network::CustomNetProtocol& protocol, int ID);

			/**
			*	
			*/
			void CloseSession( bool dissconnectClients = false ); 

			/** 
			*	Set the owner that clients will be returned to.
			*	@param owner If owner is NULL, clients will be disconnected when session is over.
			*/
			void SetOwner(NetworkSession* owner);

			/**
			*	
			*/
			virtual void ClientEventCallback(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e) = 0;

			/**
			*	
			*/
			virtual void ClientConnectedEvent(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e) = 0;

		private:
			struct PrivateSessionData;
			PrivateSessionData* data;
		};
	}
}//End namespace DanBias

#endif // !DANBIASSERVER_NETWORK_SESSION_H
