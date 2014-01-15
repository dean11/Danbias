#include "Level.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "GameMode.h"
#include "Player.h"
#include "PhysicsAPI.h"
#include "TeamManager.h"
#include "DynamicArray.h"

using namespace GameLogic;
using namespace Utility::DynamicMemory;


struct Level::PrivateData
{
	PrivateData()	
	{
		
	}
	~PrivateData()
	{
	}

	SmartPointer<TeamManager> teamManager;

	DynamicArray<SmartPointer<StaticObject>> staticObjects;
		
	DynamicArray<SmartPointer<DynamicObject>> dynamicObjects;

	SmartPointer<GameMode> gameMode;

	SmartPointer<Oyster::Physics::ICustomBody> rigidBodyLevel;

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


