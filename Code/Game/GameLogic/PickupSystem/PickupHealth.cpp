#include "PickupHealth.h"
#include "../Game.h"

using namespace GameLogic;

PickupHealth::PickupHealth(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type, int objectID)
	: Pickup(rigidBody, Pickup::PickupCollision, type, objectID, 10)
{
	this->hpValue = 50;
}

PickupHealth::~PickupHealth()
{}

void PickupHealth::OnCollision(Player *player)
{
	timer.reset();
	((Game*)&Game::Instance())->onDisableFnc(this);

	this->active = false;
	player->DamageLife(-hpValue);
}