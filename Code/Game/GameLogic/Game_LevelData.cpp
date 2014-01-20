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
Oyster::Math::Float4x4 Game::LevelData::GetOrientation()
{
	//return this->level->GetOrientation();
	return Oyster::Math::Float4x4();
}
int Game::LevelData::GetID() const
{
	//this->level->GetID();
	return -1;
}
OBJECT_TYPE Game::LevelData::GetObjectType() const
{
	//return this->level->GetType();
	return OBJECT_TYPE_UNKNOWN;
}
