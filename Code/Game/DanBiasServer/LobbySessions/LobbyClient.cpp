#include "LobbyClient.h"
#include <Protocols.h>

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;
using namespace Oyster::Callback;
using namespace GameLogic;

namespace DanBias
{
	LobbyClient::LobbyClient(SmartPointer<NetworkClient> client)
	{
		this->client = client;
		this->client->SetRecieverObject(this, NetworkProtocolCallbackType_Object);
	}
	LobbyClient::~LobbyClient()
	{
		this->callbackValue.callbackType = CallbackType_Unknown;
	}

	void LobbyClient::Disconnect()
	{
		this->client->Disconnect();
	}
	void LobbyClient::SetCallback(OysterCallback<void, NetworkSession::NetEvent> value)
	{
		this->callbackValue = value;
	}

	/** This method is NOT threadsafe. */
	void LobbyClient::NetworkCallback(CustomNetProtocol& protocol)
	{
		if(this->callbackValue.callbackType == CallbackType_Unknown) return;

		NetworkSession::NetEvent e;
		e.sender = this;
		e.protocol = protocol;

		this->callbackValue(e);
	}
	void LobbyClient::Disconnected()
	{
		if(this->callbackValue.callbackType == CallbackType_Unknown) return;

		NetworkSession::NetEvent e;
		e.sender = this;
		e.protocol = *GameLogic::Protocol_General_Status(Protocol_General_Status::States_disconected).GetProtocol();
	
		this->callbackValue(e);
	}
}//End namsapce DanBias