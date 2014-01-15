#include "Game.h"
#include "Player.h"
#include "Level.h"
#include <DynamicArray.h>

using namespace GameLogic;
using namespace Utility::DynamicMemory;

struct Game::PrivateData
{
	PrivateData()
	{

	}

	~PrivateData()
	{
		
	}
	//DynamicArray<SmartPointer<Player>> players;
	DynamicArray<SmartPointer<PlayerData>> players;
	SmartPointer<Level> level;

}myData;


Game::Game(void)
{
	myData = new PrivateData();
}

Game::~Game(void)
{
	if(myData)
	{
		delete myData;
	}
}


void Game::PlayerUseWeapon(int playerID, const WEAPON_FIRE &Usage)
{

}

void Game::GetPlayerPos(int playerID)
{
	
}

void Game::GetAllPlayerPos()
{
	
}

Game::PlayerData Game::CreatePlayer()
{
	SmartPointer<PlayerData> newPlayer = new PlayerData();

	myData->players.Push(newPlayer);

	return newPlayer;
}

void Game::CreateTeam()
{
	
}

void Game::NewFrame()
{
	
}