#ifndef NETWORK_API_NETWORK_CALLBACK_HELPER_H
#define NETWORK_API_NETWORK_CALLBACK_HELPER_H

/////////////////////////////////////
// Created by Dennis Andersen 2013 //
/////////////////////////////////////

namespace Oyster
{
	namespace Network
	{
		enum NetworkProtocolCallbackType
		{
			NetworkProtocolCallbackType_Function,
			NetworkProtocolCallbackType_Object,
			NetworkProtocolCallbackType_Unknown = -1,
		};
		enum NetworkClientCallbackType
		{
			NetworkClientCallbackType_Function,
			NetworkClientCallbackType_Object,
			NetworkClientCallbackType_Unknown = -1,
		};

		class NetworkClient;
		class CustomNetProtocol;
		typedef void (*ClientConnectCallbackMethod)(NetworkClient&);
		typedef void(*ProtocolRecieverFunction)(CustomNetProtocol& protocol);
		struct ClientConnectedObject
		{
			virtual void ClientConnectCallback(NetworkClient &client) = 0;
		};
		struct ProtocolRecieverObject
		{
			virtual void ProtocolRecievedCallback(CustomNetProtocol& protocol) = 0;
		};

		union RecieverObject
		{
			ClientConnectCallbackMethod		clientConnectFnc;
			ProtocolRecieverFunction		protocolRecieverFnc;
			ClientConnectedObject			*clientConnectObject;
			ProtocolRecieverObject			*protocolRecievedObject;
		};
	}
}

#endif