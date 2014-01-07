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


		template<typename Param>
		struct NetRecieverObject
		{
			virtual void NetworkCallback(Param) = 0;
		};
		struct NetClientEvent	:public NetRecieverObject<CustomNetProtocol&>
		{
			virtual void NetworkCallback(CustomNetProtocol& protocol) = 0;
			virtual void Disconnected() { };
		};
		typedef NetRecieverObject<NetworkClient*> ClientConnectedObject ;
		typedef NetClientEvent ProtocolRecieverObject;
		

		template <typename Param>
		struct NetCallbackFunction
		{ 
			typedef void (*FNC)(Param); 
		};
		typedef NetCallbackFunction<NetworkClient*>::FNC ClientConnectCallbackMethod;
		typedef NetCallbackFunction<CustomNetProtocol&>::FNC ProtocolRecFunction;
		struct NetClientMethods
		{
			typedef void(*Dissconnected)(void);
			ProtocolRecFunction recieved;
			Dissconnected dissconnected;
			void operator()(CustomNetProtocol& obj) { if(recieved) recieved(obj); }
			void operator()() { if(dissconnected) dissconnected(); }
		};
		typedef NetClientMethods ProtocolRecieverFunction;
		
		union RecieverObject
		{
			ClientConnectCallbackMethod		clientConnectFnc;			// !< A function pointer for sending or recieving NetworkClient
			ProtocolRecieverFunction		protocolRecieverFnc;		// !< A function pointer for sending or recieving CustomNetProtocol
			
			ClientConnectedObject			*clientConnectObject;		// !< An object for sending or recieving NetworkClient
			ProtocolRecieverObject			*protocolRecievedObject;	// !< An object with collected client events methods.

			RecieverObject()								{ memset(this, 0, sizeof(RecieverObject)); }
			RecieverObject(ClientConnectCallbackMethod	o)	{ clientConnectFnc = o; }
			RecieverObject(ProtocolRecieverFunction		o)	{ protocolRecieverFnc = o; }
			RecieverObject(ClientConnectedObject*		o)	{ clientConnectObject = o; }
			RecieverObject(ProtocolRecieverObject*		o)	{ protocolRecievedObject = o; }
		};
	}
}

#endif