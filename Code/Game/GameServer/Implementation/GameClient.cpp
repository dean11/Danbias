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


GameClient::GameClient(SmartPointer<NetworkClient> client, GameLogic::IPlayerData* player)
{
	this->client = client;
	this->player = player;
	isReady = false;
}
GameClient::~GameClient()
{
	this->client->Disconnect();
	this->player = 0;
	isReady = false;
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

float GameClient::GetSinceLastResponse() const
{
	return this->secondsSinceLastResponse;
}
bool GameClient::IsReady() const
{
	return this->isReady;
}
bool GameClient::Equals(const NetworkClient* c)
{
	return (c->GetID() == this->client->GetID());
}
void GameClient::SetReadyState(bool r)
{
	this->isReady = r;
}
void GameClient::SetSinceLastResponse(float s)
{
	this->secondsSinceLastResponse = s;
}


