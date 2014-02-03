#ifndef NETWORK_API_NETWORK_SERVER_H
#define NETWORK_API_NETWORK_SERVER_H

//////////////////////////////////////
// Created by Pontus Fransson 2013  //
// Modified by Dennis Andersen 2014 //
//////////////////////////////////////


#include "NetworkAPI_Preprocessor.h"
#include "NetworkClient.h"
#include "NetworkSession.h"
#include <vld.h>

namespace Oyster
{
	namespace Network
	{
		extern "C"
		{
			class NET_API_EXPORT NetworkServer
			{
			public:
				enum ServerReturnCode
				{
					ServerReturnCode_Error,
					ServerReturnCode_Sucess
				};

			public:
				NetworkServer();
				NetworkServer(const NetworkServer&);
				const NetworkServer& operator=(const NetworkServer&);
				virtual ~NetworkServer();


				/**	Creates a server that clients can connect to
				*	@param port	The port the server will be listening for clients.
				*	@param mainSession The main session the server will send connected clients to.
				*	@return The server returncode
				*/
				ServerReturnCode Init(const int& port, NetworkSession const* mainSession);

				/**	Starts the server allowing clients to connect
				*	@return The server returncode
				*/
				ServerReturnCode Start();

				/**	Parses asynchronous connected clients.
				*/
				int Update();

				/**	
				*	
				*/
				void Stop();

				/**	Shutdown the server and return all resources.
				*/
				void Shutdown();

				/**	Set the main session connected clients will enter when connected to server.
				*	@param mainSession The session to connect as main server session.
				*/
				void SetSession(NetworkSession const* mainSession);

				/**	Get the main session connected with the server
				*	@return Returns the main session
				*/
				NetworkSession const* GetMainSession();

				/**	Sets the main session to NULL and returns it
				*	@return Returns the main session
				*/
				NetworkSession const* ReleaseMainSession();

				/**	
				*	
				*/
				bool IsStarted() const;

				/**
				*	
				*/
				std::string GetLanAddress();

				/** Returns the port the server is listening on.
				*	@return Returns the port the server has been initiated with.
				*/
				int NetworkServer::GetPort();

			private:
				struct PrivateData;
				PrivateData* privateData;

			};
		}
	}
}

#endif