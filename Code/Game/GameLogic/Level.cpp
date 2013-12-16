#include "Level.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "GameMode.h"
#include "Player.h"

using namespace GameLogic;

struct Level::PrivateData
{
	PrivateData()
	{
		
	}
	~PrivateData()
	{
	}

	Player *players;
	int nrOfPlayers;

	StaticObject** staticObjects;
	int nrOfStaticObjects;

	DynamicObject** dynamicObjects;
	int nrOfDynamicObjects;

	GameMode* gameMode;

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


