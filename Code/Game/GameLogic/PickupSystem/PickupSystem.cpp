#include "PickupSystem.h"

using namespace GameLogic;

PickupSystem::PickupSystem()
{}

PickupSystem::~PickupSystem()
{}

void PickupSystem::CreatePickup(Pickup* pickup)
{
	pickups.push_back(pickup);
}

void PickupSystem::Update()
{
	for(int i = 0; i < (int)pickups.size(); i++)
	{
		pickups.at(i)->Update();
	}
}