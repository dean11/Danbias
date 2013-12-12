#ifndef NETWORK_API_NETWORK_CALLBACK_HELPER_H
#define NETWORK_API_NETWORK_CALLBACK_HELPER_H

/////////////////////////////////////
// Created by Dennis Andersen 2013 //
/////////////////////////////////////

#include <memory>

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
			ClientConnectCallbackMethod		clientConnectFnc;			// !< A function pointer for sending or recieving NetworkClient
			ProtocolRecieverFunction		protocolRecieverFnc;		// !< A function pointer for sending or recieving CustomNetProtocol
			ClientConnectedObject			*clientConnectObject;		// !< An object for sending or recieving NetworkClient
			ProtocolRecieverObject			*protocolRecievedObject;	// !< An object for sending or recieving CustomNetProtocol

			RecieverObject()								{ memset(this, 0, sizeof(RecieverObject)); }
			RecieverObject(ClientConnectCallbackMethod	o)	{ clientConnectFnc = o; }
			RecieverObject(ProtocolRecieverFunction		o)	{ protocolRecieverFnc = o; }
			RecieverObject(ClientConnectedObject*		o)	{ clientConnectObject = o; }
			RecieverObject(ProtocolRecieverObject*		o)	{ protocolRecievedObject = o; }
		};
	}
}

#endif