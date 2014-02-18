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


GameClient::GameClient()
{
	this->player = 0;
	isReady = false;
	this->character = L"Unknown";
	this->alias = L"Unknown";
	this->secondsSinceLastResponse = 0.0f;
}
GameClient::~GameClient()
{
	this->player = 0;
	this->isReady = false;
	this->character = L"Unknown";
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


