#include "PickupEnergy.h"
#include "../Game.h"

using namespace GameLogic;

PickupEnergy::PickupEnergy(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type, int objectID)
	: Pickup(rigidBody, Pickup::PickupCollision, type, objectID, 10)
{
	this->energyValue = 50;
}

PickupEnergy::~PickupEnergy()
{}

void PickupEnergy::OnCollision(Player *player)
{
	timer.reset();
	((Game*)&Game::Instance())->onDisableFnc(this);

	this->active = false;
	
	//Increase energy in player or weapon
}