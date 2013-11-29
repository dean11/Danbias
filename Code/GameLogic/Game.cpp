#include "Game.h"
using namespace GameLogic;

Game::Game(void)
{
	
}


Game::~Game(void)
{
	//SAFE_DELETE(player);
	if(player)
	{
		delete player;
		player = NULL;
	}
}

void Game::Init()
{
	player = new Player();
}
void Game::StartGame()
{

}
void Game::Update(keyInput keyPressed)
{
	player->Update(keyPressed);
}
void Game::Render()
{
	player->Render();
}