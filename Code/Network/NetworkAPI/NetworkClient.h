#ifndef NETWORK_API_NETWORK_CLIENT_H
#define NETWORK_API_NETWORK_CLIENT_H

//////////////////////////////////////
// Created by Pontus Fransson 2013  //
// Modified by Dennis Andersen 2014 //
//////////////////////////////////////

#include "CustomNetProtocol.h"
#include "NetworkServerEventStruct.h"
#include "NetworkAPI_Preprocessor.h"

namespace Oyster
{
	namespace Network
	{
		class NetworkSession;

		extern "C"
		{
			class NET_API_EXPORT NetworkClient
			{
			public:
				struct ClientEventArgs
				{
					enum EventType
					{
						EventType_ProtocolFailedToRecieve,		// No data
						EventType_ProtocolFailedToSend,			// Data in data.protocol
						EventType_ProtocolRecieved,				// Data in data.protocol
						EventType_Disconnect,					// No data
					} type;

					union EventData
					{
						struct { Oyster::Network::CustomNetProtocol protocol; };
						void * nothing;
					} data;
				};
				typedef void(*ClientEventFunction)(NetEvent<NetworkClient*, ClientEventArgs> e);

			public:
				NetworkClient();
				virtual ~NetworkClient();

				bool operator ==(const NetworkClient& obj);
				bool operator ==(const int& ID);

				/**
				*	
				*/
				void Update();

				/**
				*	
				*/
				bool Connect(int socket);

				/**
				*	
				*/
				bool Connect(unsigned short port, const char serverIP[]);

				/**
				*	
				*/
				void Disconnect();

				/**
				*	
				*/
				void Send(CustomProtocolObject& protocol);

				/**
				*	
				*/
				void Send(CustomNetProtocol* protocol);

				/**
				*	
				*/
				void SetOwner(NetworkSession* owner);

				/**
				*	
				*/
				bool IsConnected();
				
				/**
				*	
				*/
				int GetID() const;

				/**
				*	
				*/
				virtual void DataRecieved(NetEvent<NetworkClient*, ClientEventArgs> e);

				/**	! Deprecate ! 
				*	Do not use this furthermore, instead use void DataRecieved(NetEvent<NetworkClient*, ClientEventArgs> e);
				*	@see DataRecieved
				*/
				virtual void NetworkCallback(Oyster::Network::CustomNetProtocol& p);

			private:
				NetworkClient(const NetworkClient& obj);
				NetworkClient& operator =(const NetworkClient& obj);

				struct PrivateData;
				PrivateData* privateData;
			};
		}
	}
}

#endif