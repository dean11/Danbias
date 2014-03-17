#include "Game.h"
#include "Level.h"

using namespace GameLogic;

Game::LevelData::LevelData()
{
	this->level = new Level();
}

Game::LevelData::~LevelData()
{
	delete this->level;
	this->level = 0;
}


Oyster::Math::Float3 Game::LevelData::GetPosition()
{
	//return this->level->GetCenter();
	return Oyster::Math::Float3();
}
Oyster::Math::Quaternion Game::LevelData::GetRotation()
{
	return Oyster::Math::Quaternion();
}
Oyster::Math::Float3 Game::LevelData::GetScale()
{
	return Oyster::Math::Float3();
}
Oyster::Math::Float4x4 Game::LevelData::GetOrientation()
{
	//return this->level->GetOrientation();
	return Oyster::Math::Float4x4();
}

int Game::LevelData::GetID() const
{
	return ((IObjectData*)this->level)->GetID();
}
ObjectSpecialType Game::LevelData::GetObjectType() const
{
	return ((IObjectData*)this->level)->GetObjectType();
	//return OBJECT_TYPE_UNKNOWN;
}
bool Game::LevelData::IsDisabled() const
{
	return ((IObjectData*)this->level)->IsDisabled();
}
int Game::LevelData::getNrOfDynamicObj()const
{
	return this->level->getNrOfDynamicObj();
}
IObjectData* Game::LevelData::GetObjectAt(int ID) const
{	
	return this->level->GetObj(ID);
}

void Game::LevelData::GetAllDynamicObjects(Utility::DynamicMemory::DynamicArray<IObjectData*>& mem) const
{
	mem.Resize(level->GetDynamicObject().Size());
	for(int i = 0; i < (int)level->GetDynamicObject().Size(); i++)
	{
		mem[i] = level->GetDynamicObject()[i];
	}
}
void Game::LevelData::Update(float deltaTime)
{
	this->level->Update(deltaTime);
}
void Game::LevelData::AddPlayerToGame(IPlayerData *player)
{
	this->level->AddPlayerToGame(((PlayerData*)player)->GetPlayer());
}
void Game::LevelData::RemovePlayerFromGame(IPlayerData *player)			
{
	this->level->RemovePlayerFromGame(((PlayerData*)player)->GetPlayer());
}
void Game::LevelData::InitGameMode(float maxTimeSec, int endKillScore)
{
	this->level->InitiateGameMode(maxTimeSec, endKillScore);
}