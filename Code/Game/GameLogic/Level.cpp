#include "Level.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "GameMode.h"
#include "Player.h"
#include "PhysicsAPI.h"
#include "TeamManager.h"

using namespace GameLogic;

struct Level::PrivateData
{
	PrivateData()	
	{
		
	}
	~PrivateData()
	{
	}

	TeamManager *teamManager;

	StaticObject** staticObjects;
	int nrOfStaticObjects;

	DynamicObject** dynamicObjects;
	int nrOfDynamicObjects;

	GameMode* gameMode;

	Oyster::Physics::ICustomBody *rigidBodyLevel;

}myData;

Level::Level(void)
{
	myData = new PrivateData();
}


Level::~Level(void)
{
	delete myData;
}

void Level::InitiateLevel(std::string levelPath)
{

}

void Level::AddPlayerToTeam(Player *player, int teamID)
{
	myData->teamManager->AddPlayerToTeam(player,teamID);
}

void Level::CreateTeam(int teamSize)
{
	myData->teamManager->CreateTeam(teamSize);
}

void Level::RespawnPlayer(Player *player)
{
	myData->teamManager->RespawnPlayerRandom(player);
}


