#include "Team.h"

using namespace GameLogic;


Team::Team(void)
	:	players(5)
	,	teamSize(5)
{}

Team::Team(int teamSize)
	:	players((unsigned int)teamSize)
	,	teamSize(teamSize)
{}


Team::~Team(void)
{
	this->players.Clear();
}

Player* Team::GetPlayer(int playerID)
{
	if(playerID >= 0 && playerID < this->teamSize)
		return this->players[playerID];

	return NULL;
}

bool Team::AddPlayer(Player *player)
{
	if ((int)this->players.Size() >= this->teamSize)
	{
		return false;
	}
	else
	{
		int k = -1;
		for (int i = 0; k == -1 && i < this->teamSize; i++)
		{
			if(!this->players[i])
				k = i;
		}
		if(k == -1)
		{
			this->players.Push(player);
		}
		else
		{
			this->players[k] = player;
		}
	}

	return true;
}
