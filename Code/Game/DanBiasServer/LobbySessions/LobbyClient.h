/////////////////////////////////////////
// Created by [Dennis Andersen] [2013] //
/////////////////////////////////////////
#ifndef DANBIASSERVER_LOBBYCLIENT_H
#define DANBIASSERVER_LOBBYCLIENT_H

#include "NetworkSession.h"
#include <NetworkClient.h>
#include <PostBox\PostBox.h>

namespace DanBias
{
	class LobbyClient	:public Oyster::Network::NetClientEvent
	{
	public:
		LobbyClient(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client);
		virtual~LobbyClient();

		void Disconnect();
		void SetCallback(Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value);

		inline void Send(Oyster::Network::CustomProtocolObject& protocol)	{ this->client->Send(protocol); }
		inline void Send(Oyster::Network::CustomNetProtocol* protocol)		{ this->client->Send(protocol); }
		inline int GetID() const											{ return this->client->GetID(); }
	
	private:
		Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client;
		Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> callbackValue;

	private:
		void NetworkCallback(Oyster::Network::CustomNetProtocol& protocol)		override;
		void Disconnected()														override;

	private:
		friend class AdminInterface;
	};
}//End namspace DanBias
#endif // !DANBIASSERVER_LOBBYCLIENT_H
