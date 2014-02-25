#ifndef PICKUP_H
#define PICKUP_H

#include "../StaticObject.h"
#include "../Player.h"
#include "WinTimer.h"

typedef void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss);

namespace GameLogic
{
	class Pickup : public StaticObject
	{
	public:
		Pickup(Oyster::Physics::ICustomBody *rigidBody, EventOnCollision collisionFunc, ObjectSpecialType type, int objectID, Oyster::Math::Float spawnTime);
		virtual ~Pickup();

		virtual void Update();

		bool IsActive();

		virtual void OnCollision(Player *player) = 0;

		static void PickupCollision(Oyster::Physics::ICustomBody *rigidBodyCrate, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);

	protected:
		bool active;

		Utility::WinTimer timer;
		double spawnTime;

	};
}

#endif