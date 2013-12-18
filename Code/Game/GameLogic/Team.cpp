#include "Team.h"
#include "Player.h"

using namespace GameLogic;

struct Team::PrivateData
{
	PrivateData()
	{
		players = 0;
		nrOfPlayers = 0;
		teamSize = 0;
	}
	~PrivateData()
	{

	}
	
	Player **players;
	int nrOfPlayers;

	int teamSize;

}myData;

Team::Team(void)
{
	myData = new PrivateData();
}

Team::Team(int teamSize)
{
	myData = new PrivateData();
	myData->teamSize = teamSize;
}


Team::~Team(void)
{
	delete myData;
}

Player* Team::GetPlayer(int playerID)
{
	return myData->players[playerID];
}

bool Team::AddPlayer(Player *player)
{
	if (myData->nrOfPlayers >= myData->teamSize)
	{
		return false;
	}
	else
	{
		myData->players[myData->nrOfPlayers] = player;
		myData->nrOfPlayers++;
	}
}
