#include "Pickup.h"
#include "../Game.h"

using namespace GameLogic;

Pickup::Pickup(Oyster::Physics::ICustomBody *rigidBody, EventOnCollision collisionFunc, ObjectSpecialType type, int objectID, Oyster::Math::Float spawnTime)
	: StaticObject(rigidBody, collisionFunc, type, objectID)
{
	this->active = true;
	this->spawnTime = spawnTime;
	timer.reset();
	this->GetRigidBody()->DisableCollisionResponse();
}

Pickup::~Pickup()
{}

void Pickup::Update()
{
	if(!active)
	{
		if(timer.getElapsedSeconds() >= spawnTime)
		{
			active = true;
			((Game*)&Game::Instance())->onEnableFnc(this);
		}
	}
}

bool Pickup::IsActive()
{
	return active;
}