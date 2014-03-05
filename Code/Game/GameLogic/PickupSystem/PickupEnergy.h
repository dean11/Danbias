//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef PICKUP_ENERGY_H
#define PICKUP_ENERGY_H

#include "Pickup.h"

/****
 This pickup is meant to fill up your energy.
****/

namespace GameLogic
{
	class PickupEnergy : public Pickup
	{
	public:
		PickupEnergy(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type, int objectID);
		virtual ~PickupEnergy();

		void OnCollision(Player *player);

	protected:
		int energyValue;
	};
}

#endif