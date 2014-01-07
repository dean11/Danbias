/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "GameClient.h"
#include "..\LobbySessions\NetworkSession.h"
#include <GameProtocols.h>

using namespace Utility::DynamicMemory;
using namespace DanBias;

GameClient::GameClient(SmartPointer<LobbyClient> client, Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value)
{
	this->callbackValue = value;
	this->client = client;
	this->player = new GameLogic::Player();
	Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> c;
	c.callbackType = Oyster::Callback::CallbackType_Object;
	c.value = this;
	this->client->SetCallback(c);
	
}
GameClient::~GameClient()
{
	this->client->Disconnect();
	this->player.Release();
}

void GameClient::SetCallback(Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value)
{
	this->callbackValue = value;
}

GameLogic::Player* GameClient::GetPlayer()
{
	return this->player.Get();
}
LobbyClient* GameClient::GetClient()
{
	return this->client;
}

void GameClient::ObjectCallback(NetworkSession::NetEvent e)
{
	e.gameClient = this;
	this->callbackValue(e);
}
