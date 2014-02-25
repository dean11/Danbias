#include "PickupHealth.h"

using namespace GameLogic;

PickupHealth::PickupHealth(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type, int objectID, Oyster::Math::Float spawnTime, Oyster::Math::Float healthValue)
	: Pickup(rigidBody, Pickup::PickupCollision, type, objectID, spawnTime)
{
	this->hpValue = healthValue;
}

PickupHealth::~PickupHealth()
{}

void PickupHealth::OnCollision(Player *player)
{
	timer.reset();
	player->DamageLife(-hpValue);
}