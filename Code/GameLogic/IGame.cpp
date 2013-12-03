#include "IGame.h"
#include "Game.h"
#include <windows.h>

BOOL WINAPI DllMain(
	_In_  HINSTANCE hinstDLL,
	_In_  DWORD fdwReason,
	_In_  LPVOID lpvReserved
	)
{
	return TRUE;
}
using namespace GameLogic;

IGame::IGame()
{
	gameModule = new Game();
}
IGame::~IGame()
{
	delete gameModule;
}

void IGame::Init()
{
	gameModule->Init();
}
void IGame::StartGame()
{
	gameModule->StartGame();
}
void IGame::Update(keyInput keyPressed, float pitch, float yaw)
{
	gameModule->Update(keyPressed, pitch, yaw);
}
void IGame::Render()
{
	gameModule->Render();
}
Game* IGame::getGameModule()
{
	return gameModule;
}