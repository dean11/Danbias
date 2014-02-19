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
	int insert = InsertObject(this->players, (PlayerData*)0);
	int freeID = 0;
	bool found = false;

	for(int i = 0; i < 100; i++)
	{
		found = true;
		freeID = i;

		for(int j = 0; j < players.Size(); j++)
		{
			
			if(this->players[j] && this->players[j]->GetID() == freeID)
			{
				found = false;
			}

			if(!found) break;
		}

		if(found) break;
	}

	this->players[insert] = new PlayerData(freeID, 0); // user constructor with objectID and teamID
	this->players[insert]->player->GetRigidBody()->SetSubscription(Game::PhysicsOnMove);

	return this->players[insert];
}

Game::LevelData* Game::CreateLevel(const wchar_t mapName[255])
{
	if(this->level) return this->level;

	this->level = new LevelData();
	this->level->level->InitiateLevel(mapName);

	return this->level;
}

void Game::CreateTeam()
{
	
}

bool Game::NewFrame()
{
	for (unsigned int i = 0; i < this->players.Size(); i++)
	{
		if(this->players[i] && this->players[i]->player)	this->players[i]->player->BeginFrame();
	}

	API::Instance().UpdateWorld();

	for (unsigned int i = 0; i < this->players.Size(); i++)
	{
		if(this->players[i] && this->players[i]->player)	this->players[i]->player->EndFrame();
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

