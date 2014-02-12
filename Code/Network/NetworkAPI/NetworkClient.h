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
		struct ConnectionInfo;

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
						EventData(){}
						EventData(Oyster::Network::CustomNetProtocol& o) 
						{ 
							protocol = o; 
						}
						const EventData& operator=(EventData& o)
						{
							protocol = o.protocol;
							return *this;
						}
						const EventData& operator=(Oyster::Network::CustomNetProtocol& o) 
						{ 
							protocol = o; return *this; 
						}
					} data;
					ClientEventArgs(){}
					ClientEventArgs(ClientEventArgs& o)
					{ 
						type = o.type; 
						data = o.data; 
					}
					const ClientEventArgs& operator=(ClientEventArgs& o)
					{ 
						type = o.type; 
						data = o.data; 
						return *this; 
					}
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
				bool Connect(ConnectionInfo& data);

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
				void Send(CustomNetProtocol& protocol);

				/**
				*	
				*/
				void SetOwner(NetworkSession* owner);

				/**
				*	
				*/
				void SetMessagePump( ClientEventFunction func );

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
				//virtual void NetworkCallback(Oyster::Network::CustomNetProtocol& p);

				virtual std::string GetIpAddress();

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