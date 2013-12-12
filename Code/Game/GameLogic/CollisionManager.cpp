#include "CollisionManager.h"
#include "PhysicsAPI.h"
#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"

using namespace Oyster;

namespace GameLogic
{

	namespace CollisionManager
	{

		void PlayerVBox(Player &player, DynamicObject &box);


		Physics::ICustomBody::SubscriptMessage PlayerCollision(const Oyster::Physics::ICustomBody *rigidBodyPlayer, const Oyster::Physics::ICustomBody *obj)
		{
			Player *player = ((Player*)(rigidBodyPlayer->gameObjectRef));
			Object *realObj = (Object*)obj->gameObjectRef;

			switch (realObj->GetType())
			{
			case OBJECT_TYPE_BOX:
				PlayerVBox(*player,(*(DynamicObject*) realObj));
				break;
			case OBJECT_TYPE_PLAYER:

				break;
			}

			return Physics::ICustomBody::SubscriptMessage_none;
		}
		
		void PlayerVBox(Player &player, DynamicObject &box)
		{
			player.DamageLife(20);
		}
		
		Physics::ICustomBody::SubscriptMessage BoxCollision(const Oyster::Physics::ICustomBody *rigidBodyBox, const Oyster::Physics::ICustomBody *obj)
		{
			DynamicObject *box = (DynamicObject*)rigidBodyBox->gameObjectRef;
			Object *realObj = (Object*)obj->gameObjectRef;

			switch (realObj->GetType())
			{
			case OBJECT_TYPE_BOX:
				
				break;
			case OBJECT_TYPE_PLAYER:
				//PlayerVBox(*(Player*)realObj,*box);
				break;
			}

			return Physics::ICustomBody::SubscriptMessage_none;
		}
	}
}