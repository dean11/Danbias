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


GameClient::GameClient(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> nwClient)
{
	this->client = nwClient;
	this->player = 0;
	isReady = false;
	this->character = L"crate_colonists.dan";
	this->alias = L"Unknown";
	this->secondsSinceLastResponse = 0.0f;
}
GameClient::~GameClient()
{
	this->player = 0;
	this->isReady = false;
	this->character = L"crate_colonists.dan";
	this->alias = L"Unknown";
	this->secondsSinceLastResponse = 0.0f;
}

void GameClient::SetPlayer(GameLogic::IPlayerData* player)
{
	this->player = player;
}
void GameClient::SetReadyState(bool r)
{
	this->isReady = r;
}
void GameClient::SetSinceLastResponse(float s)
{
	this->secondsSinceLastResponse = s;
}
void GameClient::SetAlias(std::wstring alias)
{
	this->alias = alias;
}
void GameClient::SetCharacter(std::wstring character)
{
	this->character = character;
}
void GameClient::SetState(ClientState state)
{
	this->state = state;
}


void GameClient::SetOwner(Oyster::Network::NetworkSession* owner)
{
	this->client->SetOwner(owner);
}
void GameClient::UpdateClient()
{
	switch (this->state)
	{
		case ClientState_Ready:
			this->client->Update();
		break;
	}
}


IPlayerData* GameClient::ReleasePlayer()
{
	IPlayerData* temp = this->player;
	this->player = 0;
	return temp;
}
NetClient GameClient::ReleaseClient()
{
	NetClient temp = this->client;
	this->client = 0;
	return temp;
}
