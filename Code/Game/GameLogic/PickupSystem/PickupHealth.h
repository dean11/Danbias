//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef PICKUP_HEALTH_H
#define PICKUP_HEALTH_H

#include "Pickup.h"

namespace GameLogic
{
	class PickupHealth : public Pickup
	{
	public:
		PickupHealth(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type, int objectID, Oyster::Math::Float spawnTime, Oyster::Math::Float HealthValue);
		virtual ~PickupHealth();

		
		void OnCollision(Player *player);

	protected:
		int hpValue;

	};
}

#endif