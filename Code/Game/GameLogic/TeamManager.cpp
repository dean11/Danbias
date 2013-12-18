#include "TeamManager.h"
#include "Team.h"


using namespace GameLogic;

struct TeamManager::PrivateData
{
	PrivateData()
	{
		teams = 0;
		nrOfTeams = 0;
	}
	~PrivateData()
	{
	}

	Team **teams;
	int nrOfTeams;
	int maxNrOfTeams;

}myData;

TeamManager::TeamManager(void)
{
	myData = new PrivateData();
	myData->maxNrOfTeams = 10;
	myData->teams = new Team*[myData->maxNrOfTeams];
}

TeamManager::TeamManager(int maxNrOfTeams)
{
	myData = new PrivateData();
	myData->maxNrOfTeams = maxNrOfTeams;

	myData->teams = new Team*[myData->maxNrOfTeams];
	for (int i = 0; i < myData->maxNrOfTeams; i++)
	{
		myData->teams[i] = 0;
	}

}


TeamManager::~TeamManager(void)
{
	delete myData;
}

void TeamManager::RespawnPlayerRandom(Player *player)
{

	int teamID = player->GetTeamID();

	Player *respawnOnThis = myData->teams[teamID]->GetPlayer(0);

	player->Respawn(respawnOnThis->GetPos());
}

void TeamManager::CreateTeam(int teamSize)
{
	if (myData->nrOfTeams < myData->maxNrOfTeams)
	{
		myData->teams[myData->nrOfTeams] = new Team(teamSize);
		myData->nrOfTeams++;
	}
}

void TeamManager::RemoveTeam(int teamID)
{

}

bool TeamManager::AddPlayerToTeam(Player *player,int teamID)
{
	if (IsValidTeam(teamID))
	{
		return myData->teams[teamID]->AddPlayer(player);
	}
	return false;
}

bool TeamManager::AddPlayerToTeam(Player *player)
{
	
}

bool TeamManager::IsValidTeam(int teamID)
{
	if (teamID < myData->nrOfTeams && teamID > 0 && myData->teams[teamID] != NULL)
	{
		return true;
	}

	return false;
}