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
				virtual ~NetworkClient();

				bool Connect(unsigned short port, const char serverIP[]);
				void Disconnect();

				bool IsConnected();

				void Send(CustomProtocolObject& protocol);

				void SetRecieverObject(RecieverObject recvObj, NetworkProtocolCallbackType type);

			private:
				struct PrivateData;
				PrivateData* privateData;

			};
		}
	}
}

#endif