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

#pragma comment(lib, "ws2_32.lib")

//#include "NetworkClient.h"

namespace Oyster
{
	namespace Network
	{
		namespace Server
		{
			extern "C"
			{
				class NET_PROTOCOL_EXPORT NetworkServer
				{
				public:
					class NetworkClient;
					struct INIT_DESC
					{
						unsigned short port;		//Port the server should be accepting clients on.
						void (*proc)(NetworkClient*);
					};

					NetworkServer();
					virtual ~NetworkServer();

					virtual bool Init(INIT_DESC& initDesc);
					virtual bool Start();
					virtual bool Stop();
					virtual bool Shutdown();

					virtual bool IsStarted() const;

				private:
					struct PrivateData;
					PrivateData* privateData;

				};
			}
		}
	}
}

#endif