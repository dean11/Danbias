#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Object.h"
#include "PhysicsAPI.h"
#include "RefManager.h"
#include "DynamicObject.h"
#include "Player.h"

namespace GameLogic
{
	RefManager refManager;

	namespace CollisionManager
	{

			void ColisionEvent(Oyster::Physics::ICustomBody &obj1, Oyster::Physics::ICustomBody &obj2);
			void PlayerVBox(Player &player, DynamicObject &box);

	};

}

#endif