#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Object.h"
#include "PhysicsAPI.h"

namespace GameLogic
{

	class CollisionManager
	{
	public:
		//put general collision functions here that are not part of a specific object
		static Oyster::Physics::ICustomBody::SubscriptMessage IgnoreCollision(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody *obj);
			
	};

}

#endif