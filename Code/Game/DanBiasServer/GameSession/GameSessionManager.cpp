/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "GameSessionManager.h"

#include "..\LobbySessions\LobbyClient.h"
#include "GameSession.h"
#include <DynamicArray.h>

using namespace DanBias;
using namespace Utility::DynamicMemory;

struct GameSessionData
{
	DynamicArray< SmartPointer< GameSession > > sessions;

	int freeSpot;

	int Existst(int session)
	{
		for (unsigned int i = 0; i < sessions.Size(); i++)
		{
			if(!sessions[i] && freeSpot == -1)		freeSpot = i;
			if(sessions[i]->GetID() == session)		return i;
		}
		return -1;
	}
	int GetFree()
	{
		for (unsigned int i = 0; i < sessions.Size(); i++)
		{
			if(!sessions[i])
			{
				this->freeSpot = i;
				return this->freeSpot;
			}
		}

		this->freeSpot = -1;
		return this->freeSpot;
	}

} gameSessionData;


int GameSessionManager::AddSession(GameSessionDescription& instance, bool run)
{
	int k = gameSessionData.GetFree();

	SmartPointer<GameSession> gs = new GameSession();

	DanBias::GameSession::GameDescription desc;
	desc.owner = instance.exitDestionation;
	desc.clients = instance.clients;
	desc.mapName = instance.mapName;
	

	if(!gs->Create(desc)) return 0;

	if(k == -1) gameSessionData.sessions.Push(gs);
	else		gameSessionData.sessions[k] = gs;

	if(run)		gs->Run();

	return gs->GetID();
}

bool GameSessionManager::StartSession(int session)
{
	int i = -1;
	if((i = gameSessionData.Existst(session)) != -1) return false;

	gameSessionData.sessions[i]->Run();

	return true;
}

bool GameSessionManager::JoinSession(int session, Utility::DynamicMemory::SmartPointer<LobbyClient> client)
{
	int i = -1;
	if((i = gameSessionData.Existst(session)) != -1) return false;

	gameSessionData.sessions[i]->Join(client);

	return true;
}

void GameSessionManager::GetSessionInfo(int session, GameSessionInfo& data)
{
	memset(&data, 0, sizeof(GameSessionInfo));

	int i = -1;
	if((i = gameSessionData.Existst(session)) != -1) return;

	//data.gametime			= gameSessionData.sessions[i]->
	//data.mapName			= gameSessionData.sessions[i]->
	//data.numberOfPlayers	= gameSessionData.sessions[i]->
}

void GameSessionManager::CloseSession(int session)
{
	int i = -1;
	if((i = gameSessionData.Existst(session)) != -1) return;
	

	gameSessionData.sessions[i]->CloseSession();
}

int GameSessionManager::GetSessionSize()
{
	return gameSessionData.sessions.Size();
}




