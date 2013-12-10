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
		//these are the main collision functions
		//typedef SubscriptMessage (*EventAction_Collision)( const ICustomBody *proto, const ICustomBody *deuter );
		Oyster::Physics::ICustomBody::SubscriptMessage PlayerCollision(const Oyster::Physics::ICustomBody *rigidBodyPlayer, const Oyster::Physics::ICustomBody *obj);
		Oyster::Physics::ICustomBody::SubscriptMessage BoxCollision(const Oyster::Physics::ICustomBody *rigidBodyBox, const Oyster::Physics::ICustomBody *obj);

		//these are the specific collision case functions
		void PlayerVBox(Player &player, DynamicObject &box);
		void BoxVBox(DynamicObject &box1, DynamicObject &box2);
			
	};

}

#endif