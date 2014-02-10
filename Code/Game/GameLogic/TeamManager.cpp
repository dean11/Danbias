#include "TeamManager.h"

using namespace GameLogic;


TeamManager::TeamManager(void)
	:	teams(10)
	,	maxNrOfTeams(10)
{
	for (int i = 0; i < 10; i++)
	{
		teams[i] = 0;
	}
}

TeamManager::TeamManager(int maxNrOfTeams)
	:	teams(maxNrOfTeams)
	,	maxNrOfTeams(maxNrOfTeams)
{
	for (int i = 0; i < this->maxNrOfTeams; i++)
	{
		teams[i] = 0;
	}
}


TeamManager::~TeamManager(void)
{
	for (int i = 0; i < this->maxNrOfTeams; i++)
	{
		delete this->teams[i];
		this->teams[i] = 0;
	}
}

void TeamManager::RespawnPlayerRandom(Player *player)
{
	int teamID = player->GetTeamID();

	Player *respawnOnThis = this->teams[teamID]->GetPlayer(0);

	player->Respawn(respawnOnThis->GetPosition());
}

void TeamManager::CreateTeam(int teamSize)
{
	//if (this->nrOfTeams < this->maxNrOfTeams)
	//{
	//	this->teams[this->nrOfTeams] = new Team(teamSize);
	//	this->nrOfTeams++;
	//}
}

void TeamManager::RemoveTeam(int teamID)
{

}

bool TeamManager::AddPlayerToTeam(Player *player,int teamID)
{
	//if (IsValidTeam(teamID))
	//{
	//	return this->teams[teamID]->AddPlayer(player);
	//}
	return false;
}

bool TeamManager::AddPlayerToTeam(Player *player)
{
	return false;
}

bool TeamManager::IsValidTeam(int teamID)
{
	//if (teamID < this->nrOfTeams && teamID > 0 && this->teams[teamID] != NULL)
	//{
	//	return true;
	//}

	return false;
}