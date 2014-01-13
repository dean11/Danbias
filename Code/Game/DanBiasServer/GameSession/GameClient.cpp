/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "GameClient.h"
#include "..\LobbySessions\NetworkSession.h"
#include <Game.h>
#include <GameProtocols.h>

using namespace Utility::DynamicMemory;
using namespace DanBias;
using namespace GameLogic;

static int gameClientIDCount = 1;

GameClient::GameClient(SmartPointer<LobbyClient> client, Game::PlayerData player, Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value)
{
	this->callbackValue = value;
	this->client = client;
	//this->player = new GameLogic::Player();
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
	this->player.playerID = 0;
	this->player.teamID = 0;
	this->id = -1;
	//this->player.Release();
}

void GameClient::SetCallback(Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value)
{
	this->callbackValue = value;
}

//GameLogic::Player* GameClient::GetPlayer()
//{
//	return this->player.Get();
//}
GameLogic::Game::PlayerData* GameClient::GetPlayer()
{
	return &this->player;
}
GameLogic::Game::PlayerData GameClient::ReleasePlayer()
{
	GameLogic::Game::PlayerData temp = this->player;
	this->player.playerID = 0;
	this->player.teamID = 0;
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
