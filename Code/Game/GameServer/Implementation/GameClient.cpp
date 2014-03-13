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
	this->isInvalid = false;
	this->failedPackagesCount = 0;
	this->client = nwClient;
	this->player = 0;
	this->isReady = false;
	this->character = L"char_orca.dan";
	this->alias = L"Unknown";
	this->secondsSinceLastResponse = 0.0f;
	this->state = ClientState_Ready;
}
GameClient::~GameClient()
{
	this->client->Disconnect();
 	if(this->player)
	{
 		this->player->Inactivate();
	}

	this->isReady = false;
	this->character = L"char_orca.dan";
	this->alias = L"Unknown";
	this->secondsSinceLastResponse = 0.0f;
	this->client = 0;
	this->player = 0;
}

void GameClient::SetPlayer(GameLogic::IPlayerData* player)
{
	if(this->player)
		this->player->Inactivate();
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

bool GameClient::IsInvalid()
{
	return this->isInvalid;
}
void GameClient::Invalidate()
{
	this->client->Disconnect();
	this->isReady = false;
	this->isInvalid = true;
	this->secondsSinceLastResponse = 0.0f;
	this->failedPackagesCount = 0;
	this->character = L"char_orca.dan";
	this->alias = L"Unknown";
	this->state = ClientState_Invalid;
	this->player->Release();
	this->player = 0;
	
}
int GameClient::IncrementFailedProtocol()
{
	this->failedPackagesCount++;
	return this->failedPackagesCount;
}
void GameClient::ResetFailedProtocolCount()
{
	this->failedPackagesCount = 0;
}

void GameClient::SetOwner(Oyster::Network::NetworkSession* owner)
{
	this->client->SetOwner(owner);
}
void GameClient::UpdateClient()
{
	switch (this->state)
	{
		case ClientState_CreatingGame:
		case ClientState_Ready:
			this->client->Update();
		break;
	}
}


void GameClient::ReleasePlayer()
{
	this->player->Inactivate();
	this->player = nullptr;
}
NetClient GameClient::ReleaseClient()
{
	NetClient temp = this->client;
	this->client = 0;
	return temp;
}
