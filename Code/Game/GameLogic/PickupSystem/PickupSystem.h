//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef PICKUP_SYSTEM_H
#define PICKUP_SYSTEM_H

#include <vector>

#include "Pickup.h"

namespace GameLogic
{
	class PickupSystem
	{
	public:
		PickupSystem();
		~PickupSystem();

		void CreatePickup(Pickup* pickup);

		void Update();

	private:
		std::vector<Utility::DynamicMemory::SmartPointer<Pickup>> pickups;

	};
}

#endif