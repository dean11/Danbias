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
	return ((IObjectData*)this->level)->GetID();
}
OBJECT_TYPE Game::LevelData::GetObjectType() const
{
	return ((IObjectData*)this->level)->GetObjectType();
	//return OBJECT_TYPE_UNKNOWN;
}

IObjectData* Game::LevelData::GetObjectAt(int ID) const
{	
	return this->level->GetObj(ID);
}