#include "GameMode.h"

using namespace GameLogic;


struct GameMode::PrivateData
{
	PrivateData()
	{

	}

	~PrivateData()
	{

	}

}myData;


GameMode::GameMode()
{
	myData = new PrivateData();
}


GameMode::~GameMode(void)
{
	delete myData;
}
