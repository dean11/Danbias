/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "..\GameClient.h"
#include <NetworkSession.h>
#include <Protocols.h>

using namespace Utility::DynamicMemory;
using namespace DanBias;
using namespace GameLogic;

static int gameClientIDCount = 1;

GameClient::GameClient(Oyster::Network::NetworkClient client, GameLogic::IPlayerData* player)
{
	this->client = client;
	this->id = gameClientIDCount++;
	this->player = player;
}
GameClient::~GameClient()
{
	this->client.Disconnect();
	this->player = 0;
	this->id = -1;
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
Oyster::Network::NetworkClient* GameClient::GetClient()
{
	return &this->client;
}
Oyster::Network::NetworkClient GameClient::ReleaseClient()
{
	Oyster::Network::NetworkClient temp = this->client;
	return temp;
}
int GameClient::GetID() const
{
	return this->id;
}
