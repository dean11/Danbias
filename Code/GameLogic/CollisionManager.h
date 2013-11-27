#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Object.h"
#include "PhysicsAPI.h"
#include "RefManager.h"
#include "DynamicObject.h"
#include "Player.h"

namespace GameLogic
{

	class CollisionManager
	{

		public:
			CollisionManager(void);
			~CollisionManager(void);

		private:
			void ColisionEvent(Oyster::Physics::ICustomBody &obj1, Oyster::Physics::ICustomBody &obj2);
			void PlayerVBox(Player &player, DynamicObject &box);

		private:
			RefManager *refManager;

	};

}

#endif