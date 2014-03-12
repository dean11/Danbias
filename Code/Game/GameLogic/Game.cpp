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
	,	onDamageTakenFnc(0)
	,   onRespawnFnc(0)
	,	onDeadFnc(0)
	,	frameTime(1.0f/120.0f)
	,	timer(0.0f)
	,	level(0)
	,	tickPeriod(1.0f)
	,	tickTimer(0.0f)
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
	//Se if there is a free player somewhere in our list
	for (unsigned int i = 0; i < this->players.Size(); i++)
	{
		if(this->players[i] && this->players[i]->player->IsReleased())
		{
			//We give the body to someone else
			this->players[i]->player->Activate();
			this->level->AddPlayerToGame(this->players[i]);
			return this->players[i];
		}
	}

	// Find a free space in array or insert at end
	int insert = InsertObject(this->players, (PlayerData*)0);
	int freeID = 0;
	bool found = false;

	for(int i = 0; i < 100; i++)
	{
		found = true;
		freeID = i;

		for(int j = 0; j < (int)players.Size(); j++)
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

	this->level->AddPlayerToGame(this->players[insert]);
	return this->players[insert];
}

void Game::RemovePlayer( IPlayerData* player)
{
	this->level->RemovePlayerFromGame(player);
}
Game::LevelData* Game::CreateLevel(const wchar_t mapName[255])
{
	if(this->level) return this->level;

	this->level = new LevelData();
	this->level->level->InitiateLevel(mapName);

	return this->level;
}
void Game::Release()
{
	for (unsigned int i = 0; i < this->players.Size(); i++)
	{
		delete this->players[i];
	}
	this->players.Clear();

	delete this->level;
	this->level = 0;

	initiated = false;

	API::Instance().Release();
}

void Game::CreateTeam()
{
	
}

bool Game::NewFrame(float deltaTime)
{
	this->timer += deltaTime;
	if(this->timer >= this->frameTime)
	{
		this->level->Update(this->timer);

		for (unsigned int i = 0; i < this->players.Size(); i++)
		{
			if(this->players[i] && this->players[i]->player)	this->players[i]->player->BeginFrame(this->timer);
		}

		API::Instance().UpdateWorld(this->timer);
	
		for (unsigned int i = 0; i < this->players.Size(); i++)
		{
			if(this->players[i] && this->players[i]->player)	this->players[i]->player->EndFrame();
		}
		this->timer = 0;
	}
	return true;
}

void Game::SetFPS(int FPS)
{
	this->SetFrameTimeLength( 1.0f /(float) FPS );
}

void Game::SetFrameTimeLength( float seconds )
{
	this->frameTime = seconds;
	::Oyster::Physics::API::Instance().SetTimeStep( seconds );
}

void Game::SetMoveSubscription(GameEvent::ObjectMovedFunction functionPointer)
{
	this->onMoveFnc = functionPointer;
}
void Game::SetDisableSubscription(GameEvent::ObjectDisabledFunction functionPointer)
{
	this->onDisableFnc = functionPointer;
}
void Game::SetEnableSubscription(GameEvent::ObjectEnabledFunction functionPointer)
{
	this->onEnableFnc = functionPointer;
}
void Game::SetHpSubscription(GameEvent::ObjectHpFunction functionPointer)
{
	this->onDamageTakenFnc = functionPointer;
}
void Game::SetRespawnSubscription(GameEvent::ObjectRespawnedFunction functionPointer)
{
	this->onRespawnFnc = functionPointer;
}
void Game::SetDeadSubscription(GameEvent::ObjectDeadFunction functionPointer)
{
	this->onDeadFnc = functionPointer;
}
void Game::SetActionSubscription(GameEvent::AnimationEventFunction functionPointer)
{
	this->onActionEventFnc = functionPointer;
}
void Game::SetPickupSubscription(GameEvent::PickupEventFunction functionPointer)
{
	this->onPickupEventFnc = functionPointer;
}
void Game::SetCollisionSubscription(GameEvent::CollisionEventFunction functionPointer) 
{
	this->onCollisionEventFnc = functionPointer;
}
void Game::SetWeaponEnergySubscription(GameEvent::WeaponEnergyFunction functionPointer) 
{
	this->onEnergyUpdateFnc = functionPointer;
}
void Game::SetGameOverSubscription(GameEvent::EndGameFunction functionPointer) 
{
	this->onEndGameFnc = functionPointer;
}
void Game::SetBeamEffectSubscription(GameEvent::BeamEffectFunction functionPointer)
{
	this->onBeamEffectFnc = functionPointer;
}
void Game::SetOnGameTimeTick(GameEvent::OnGameTimeTickFunction functionPointer, float period)
{
	this->tickPeriod = period;
	this->onGameTimeTickFnc = functionPointer;
}
bool Game::Initiate()
{
	API::Instance().Init();
	//API::Instance().SetSubscription(Game::PhysicsOnDestroy);
	API::Instance().SetTimeStep(this->frameTime);
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
	if(gameInstance.onDisableFnc) gameInstance.onDisableFnc(0);
}
