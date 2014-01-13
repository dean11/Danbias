#include "Game.h"
#include "Player.h"
#include "Level.h"
#include <GID.h>

using namespace GameLogic;

struct Game::PrivateData
{
	PrivateData()
	{

	}

	~PrivateData()
	{
		
	}

	Player **players;
	Level *level;

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

void Game::MovePlayer(int playerID, const PLAYER_MOVEMENT &movement)
{
	
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
	PlayerData data;
	//Yo mammma
	data.playerID = GID();
	data.teamID = -1;

	return data;
}

void Game::CreateTeam()
{
	
}

void Game::NewFrame()
{
	
}