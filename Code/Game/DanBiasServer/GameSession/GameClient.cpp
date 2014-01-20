/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "GameClient.h"
#include "..\LobbySessions\NetworkSession.h"
#include <Protocols.h>

using namespace Utility::DynamicMemory;
using namespace DanBias;
using namespace GameLogic;

static int gameClientIDCount = 1;

GameClient::GameClient(SmartPointer<LobbyClient> client, IPlayerData* player, Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value)
{
	this->callbackValue = value;
	this->client = client;
	this->id = gameClientIDCount++;
	this->player = player;
	Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> c;
	c.callbackType = Oyster::Callback::CallbackType_Object;
	c.value = this;
	this->client->SetCallback(c);
	
}
GameClient::~GameClient()
{
	if(this->client) this->client->Disconnect();
	this->player = 0;
	this->id = -1;
}

void GameClient::SetCallback(Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value)
{
	this->callbackValue = value;
}

GameLogic::IPlayerData* GameClient::GetPlayer()
{
	return this->player;
}
GameLogic::IPlayerData* GameClient::ReleasePlayer()
{
	GameLogic::IPlayerData *temp = this->player;
	this->player =  0;
	return temp;
}
LobbyClient* GameClient::GetClient() const
{
	return this->client;
}
Utility::DynamicMemory::SmartPointer<LobbyClient> GameClient::ReleaseClient()
{
	SmartPointer<LobbyClient> temp = this->client;
	this->client = 0;
	return temp;
}
int GameClient::GetID() const
{
	return this->id;
}
void GameClient::ObjectCallback(NetworkSession::NetEvent e)
{
	e.gameClient = this;
	this->callbackValue(e);
}
