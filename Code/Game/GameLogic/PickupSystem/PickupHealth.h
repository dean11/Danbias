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
		PickupHealth(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type, int objectID);
		virtual ~PickupHealth();

		
		void OnCollision(Player *player);

	protected:
		int hpValue;

	};
}

#endif