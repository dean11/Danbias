#include "Game.h"
#include "Player.h"
#include "Level.h"
#include <DynamicArray.h>
#include <GID.h>
#include <PhysicsAPI.h>
#include <WinTimer.h>

using namespace GameLogic;
using namespace Utility::DynamicMemory;
using namespace Oyster::Physics;

#define DELTA_TIME	0.01666666666666666666666666666667f

template<typename T>
int InsertObject(DynamicArray<T*>& list, T* obj)
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
template<typename T>
int RemoveObject(DynamicArray<T*>& list, T* obj)
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

struct Game::PrivateData
{
	PrivateData()
	{  }

	~PrivateData()
	{
		for (unsigned int i = 0; i < players.Size(); i++)
		{
			this->players[i]->player = 0;
		}
	}

	DynamicArray<PlayerData*> players;
	SmartPointer<Level> level;
	Utility::WinTimer timer;

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

void Game::GetAllPlayerPositions() const
{
	
}

Game::PlayerData* Game::CreatePlayer()
{
	Player *newPlayer = new Player();
	PlayerData *newPdata = new PlayerData();
	newPdata->player = newPlayer;
	int id = InsertObject(this->myData->players, newPdata);
	return this->myData->players[id];
}

void Game::CreateTeam()
{
	
}

void Game::NewFrame()
{
	double dt = this->myData->timer.getElapsedSeconds();

	//60 fps sec is currently staticly
}

	if(dt >= DELTA_TIME)
	{
		for (int i = 0; i < this->myData->players.Size(); i++)
		{
			if(this->myData->players[i]->player)
				this->myData->players[i]->player->BeginFrame();
		}
		API::Instance().Update();

		for (int i = 0; i < this->myData->players.Size(); i++)
		{
			if(this->myData->players[i]->player)
				this->myData->players[i]->player->EndFrame();
		}
		this->myData->timer.reset();
	}
}