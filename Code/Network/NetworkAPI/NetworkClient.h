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

class RecieverObject;

namespace Oyster
{
	namespace Network
	{
		extern "C"
		{
			class NET_PROTOCOL_EXPORT NetworkClient
			{
			public:
				NetworkClient();
				virtual ~NetworkClient();

				virtual void Disconnect();
				virtual bool IsConnected();

				virtual void Send();

			private:
				struct PrivateData;
				PrivateData* privateData;

			};
		}
	}
}

#endif