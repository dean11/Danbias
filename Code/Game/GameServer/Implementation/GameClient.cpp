/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "..\GameClient.h"
#include <NetworkSession.h>
#include <Protocols.h>

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;
using namespace DanBias;
using namespace GameLogic;

static int gameClientIDCount = 1;

GameClient::GameClient(SmartPointer<NetworkClient> client, GameLogic::IPlayerData* player)
{
	this->client = client;
	this->id = gameClientIDCount++;
	this->player = player;
}
GameClient::~GameClient()
{
	this->client->Disconnect();
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
SmartPointer<Oyster::Network::NetworkClient> GameClient::GetClient()
{
	return this->client;
}
SmartPointer<Oyster::Network::NetworkClient> GameClient::ReleaseClient()
{
	SmartPointer<Oyster::Network::NetworkClient> temp = this->client;
	this->client = 0;
	return temp;
}
int GameClient::GetID() const
{
	return this->id;
}
bool GameClient::Equals(const NetworkClient* c)
{
	return (c->GetID() == this->client->GetID());
}


