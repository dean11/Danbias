#include "Game.h"
using namespace GameLogic;

Game::Game(void)
{
	
}


Game::~Game(void)
{
	SAFE_DELETE(player);
}

void Game::Init()
{
	player = new Player();
}
void Game::StartGame()
{

}
void Game::Update()
{
	player->Update();
}
void Game::Render()
{
	player->Render();
}