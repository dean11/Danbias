#ifndef NETWORK_API_NETWORK_CLIENT_H
#define NETWORK_API_NETWORK_CLIENT_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#ifdef CUSTOM_NET_PROTOCOL_EXPORT
	#define NET_PROTOCOL_EXPORT __declspec(dllexport)
#else
	#define NET_PROTOCOL_EXPORT __declspec(dllimport)
#endif

#include "NetworkCallbackHelper.h"
#include <vld.h>

namespace Oyster
{
	namespace Network
	{
		extern "C"
		{
			struct CustomProtocolObject;
			class NET_PROTOCOL_EXPORT NetworkClient
			{
			public:
				NetworkClient();
				NetworkClient(unsigned int socket);
				NetworkClient(RecieverObject recvObj, NetworkProtocolCallbackType type);
				NetworkClient(RecieverObject recvObj, NetworkProtocolCallbackType type, unsigned int socket);
				NetworkClient(const NetworkClient& obj);
				
				NetworkClient& operator =(const NetworkClient& obj);
				virtual ~NetworkClient();

				bool Connect(unsigned short port, const char serverIP[]);
				void Disconnect();

				bool IsConnected();

				//Adds the protocol to the queue of protocols to be sent.
				void Send(CustomProtocolObject& protocol);

				void SetRecieverObject(RecieverObject recvObj, NetworkProtocolCallbackType type);

				//Compares the internal ID.
				bool operator ==(const NetworkClient& obj);

			private:
				struct PrivateData;
				PrivateData* privateData;

			};
		}
	}
}

#endif