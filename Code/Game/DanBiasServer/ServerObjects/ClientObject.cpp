#include "ClientObject.h"

using namespace DanBias;

ClientObject::ClientObject(Oyster::Network::NetworkClient* client)
{
	this->client = client;
	this->client->SetRecieverObject(this, Oyster::Network::NetworkProtocolCallbackType_Object);
	this->box = 0;
}
ClientObject::~ClientObject()
{
	this->client->Disconnect();
}
void ClientObject::SetProtocolCallback(Oyster::Network::ProtocolRecieverObject* object)
{
	this->GetClient()->SetRecieverObject(object, Oyster::Network::NetworkProtocolCallbackType_Object);
}
void ClientObject::SetPostbox(Oyster::IPostBox<NetworkSession::NetEvent>* box)
{
	this->box = box;
}
GameLogic::Player* ClientObject::GetPlayer()
{
	return this->player.Get();
}
Oyster::Network::NetworkClient* ClientObject::GetClient()
{
	return this->client.Get();
}

void ClientObject::CreatePlayer()
{
	if(this->player) return;

	this->player = new GameLogic::Player();
}
void ClientObject::ErasePlayer()
{
	while(this->player.Release());
}

void ClientObject::ProtocolRecievedCallback(Oyster::Network::CustomNetProtocol& protocol)
{
	if(!this->box) return;

	NetworkSession::NetEvent _event;
	_event.protocol = protocol;
	_event.reciever = this;

	this->box->Post(_event);
}

