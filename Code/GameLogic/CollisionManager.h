#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Object.h"
#include "PhysicsAPI.h"
#include "RefManager.h"
#include "DynamicObject.h"
#include "Player.h"

namespace GameLogic
{

	namespace CollisionManager
	{
		void PlayerCollision(Oyster::Physics::ICustomBody &rigidBodyPlayer,Oyster::Physics::ICustomBody &obj);
		void BoxCollision(Oyster::Physics::ICustomBody &rigidBodyBox, Oyster::Physics::ICustomBody &obj);



		void PlayerVBox(Player &player, DynamicObject &box);
		void BoxVBox(DynamicObject &box1, DynamicObject &box2);
			
	};

}

#endif