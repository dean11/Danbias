#ifndef NETWORK_API_NETWORK_SERVER_H
#define NETWORK_API_NETWORK_SERVER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#ifdef CUSTOM_NET_PROTOCOL_EXPORT
	#define NET_PROTOCOL_EXPORT __declspec(dllexport)
#else
	#define NET_PROTOCOL_EXPORT __declspec(dllimport)
#endif

#include "NetworkClient.h"
#include "NetworkCallbackHelper.h"

namespace Oyster
{
	namespace Network
	{
		extern "C"
		{
			class NET_PROTOCOL_EXPORT NetworkServer
			{
			public:
				struct INIT_DESC
				{
					unsigned short port;		//Port the server should be accepting clients on.
					
					NetworkClientCallbackType callbackType; //The recieverObject type. Function or object.
					RecieverObject recvObj;		//The functions that is called when a new client has connected.
				};

				NetworkServer();
				virtual ~NetworkServer();

				bool Init(INIT_DESC& initDesc);
				bool Start();
				void Stop();
				void Shutdown();

				bool IsStarted() const;

			private:
				struct PrivateData;
				PrivateData* privateData;

			};
		}
	}
}

#endif