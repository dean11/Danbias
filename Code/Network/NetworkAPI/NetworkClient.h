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
				bool Connect(unsigned short port, std::wstring serverIP);

				/**Tries to connect with the same port and ip it earlier used for Connect.
				*	
				*/
				bool Reconnect();

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

				/**
				*	
				*/
				std::string GetIpAddress();

				/**	Dumps all activity to std::io
				*	
				*/
				void OutputEventData(bool output);

				/**	Dumps all activity to std::io
				*	
				*/
				void SetOutputEventDataStream(FILE out);


				/** Starts a seperate thread for listening to broadcast messages.
				* @param port: The port to listen for messages on.
				* @return Returns true if broadcasting is already started or if it started now.
						Returns false if it failed to start broadcasting.
				*/
				bool StartListeningForBroadcasting(unsigned short port);

				/** Stops the seperate thread for broadcast messages.
				*
				*/
				void StopListeningForBroadcasting();

			private:
				NetworkClient(const NetworkClient& obj);
				NetworkClient& operator =(const NetworkClient& obj);

				ClientEventFunction OnRecieve;
				struct PrivateData;
				PrivateData* privateData;
			};
		}
	}
}

#endif