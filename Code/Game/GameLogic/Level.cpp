#include "Level.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "GameMode.h"

using namespace GameLogic;

struct Level::PrivateData
{
	PrivateData()
	{
		
	}
	~PrivateData()
	{
	}


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


