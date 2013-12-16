#include "ClientObject.h"

using namespace DanBias;

ClientObject::ClientObject(const Oyster::Network::NetworkClient& client)
{
	this->client = client;
	this->client.SetRecieverObject(this, Oyster::Network::NetworkProtocolCallbackType_Object);
	this->box = 0;
}
ClientObject::~ClientObject()
{
	this->client.Disconnect();
}

void ClientObject::SetPostbox(Oyster::PostBox<NetworkSession::ClientEvent>* box)
{
	this->box = box;
}
GameLogic::Player* ClientObject::Logic_Object()
{
	return &this->logicPlayer;
}
Oyster::Network::NetworkClient* ClientObject::NetClient_Object()
{
	return &this->client;
}

void ClientObject::ProtocolRecievedCallback(Oyster::Network::CustomNetProtocol& protocol)
{
	if(!this->box) return;

	NetworkSession::ClientEvent _event;
	_event.protocol = protocol;
	_event.reciever = this;

	this->box->Post(_event);
}

