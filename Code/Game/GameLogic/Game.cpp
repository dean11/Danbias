#include "Game.h"

using namespace GameLogic;
using namespace Utility::DynamicMemory;
using namespace Oyster::Physics;

template<typename T> int InsertObject(DynamicArray<T*>& list, T* obj)
{
	for (unsigned int i = 0; i < list.Size(); i++)
	{
		if(!list[i])
		{
			list[i] = obj;
			return i;
		}
	}	
	list.Push(obj);
	return list.Size() - 1;
}
template<typename T> int RemoveObject(DynamicArray<T*>& list, T* obj)
{
	for (unsigned int i = 0; i < list.Size(); i++)
	{
		if(!list[i])
		{
			list[i] = obj;
			return i;
		}
	}
	list.Push(obj);
	return list.Size() - 1;
}


Game gameInstance;
GameAPI& GameAPI::Instance()
{
	return gameInstance;
}


Game::Game(void)
	:	initiated(false)
	,	onMoveFnc(0)
	,	onDisableFnc(0)
	,	frameTime(1.0f/120.0f)
{}

Game::~Game(void)
{ 
	for (unsigned int i = 0; i < gameInstance.players.Size(); i++)
	{
		delete gameInstance.players[i];
	}
	gameInstance.players.Clear();

	delete this->level;
	this->level = 0;

	initiated = false;
}

void Game::GetAllPlayerPositions() const
{
	
}

Game::PlayerData* Game::CreatePlayer()
{
	// Find a free space in array or insert at end
	int i = InsertObject(this->players, (PlayerData*)0);

	this->players[i] = new PlayerData();
	this->players[i]->player->GetRigidBody()->SetSubscription(Game::PhysicsOnMove);

	return this->players[i];
}

Game::LevelData* Game::CreateLevel()
{
	if(this->level) return this->level;

	this->level = new LevelData();
	//this->level->level->InitiateLevel(1000);
	this->level->level->InitiateLevel("3bana.bias");

	return this->level;
}

void Game::CreateTeam()
{
	
}

bool Game::NewFrame()
{
	for (unsigned int i = 0; i < this->players.Size(); i++)
	{
		if(this->players[i]->player)	this->players[i]->player->BeginFrame();
	}

	API::Instance().UpdateWorld();

	for (unsigned int i = 0; i < this->players.Size(); i++)
	{
		if(this->players[i]->player)	this->players[i]->player->EndFrame();
		gameInstance.onMoveFnc(this->players[i]);
	}
	for (unsigned int i = 0; i < this->level->level->dynamicObjects.Size(); i++)
	{
		gameInstance.onMoveFnc(this->level->level->dynamicObjects[i]);
	}
	

	return true;
}

void Game::SetFPS(int FPS)
{
	this->frameTime = 1.0f / FPS;
}

void Game::SetFrameTimeLength( float seconds )
{
	this->frameTime = seconds;
}

void Game::SetSubscription(GameEvent::ObjectMovedFunction functionPointer)
{
	this->onMoveFnc = functionPointer;
}
void Game::SetSubscription(GameEvent::ObjectDisabledFunction functionPointer)
{
	this->onDisableFnc = functionPointer;

}

bool Game::Initiate()
{
	API::Instance().Init();
	//API::Instance().SetSubscription(Game::PhysicsOnDestroy);
	//API::Instance().SetFrameTimeLength(this->frameTime);
	this->initiated = true;
	return true;
}


float Game::GetFrameTime() const
{
	return this->frameTime;
}

/**********************************************/
/*********** Private methods ******************/
/***************************************************************************************************/

void Game::PhysicsOnMove(const ICustomBody *object)
{
	IObjectData* temp = (IObjectData*)object->GetCustomTag();
		
	if(gameInstance.onMoveFnc && temp) gameInstance.onMoveFnc(temp);
}
void Game::PhysicsOnDestroy(::Utility::DynamicMemory::UniquePointer<ICustomBody> proto)
{
	if(gameInstance.onDisableFnc) gameInstance.onDisableFnc(0, 0);
}

