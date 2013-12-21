#include "CollisionManager.h"
#include "PhysicsAPI.h"
#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"

using namespace Oyster;

using namespace GameLogic;

	void PlayerVBox(Player &player, DynamicObject &box);


	Physics::ICustomBody::SubscriptMessage CollisionManager::PlayerCollision(const Oyster::Physics::ICustomBody *rigidBodyPlayer, const Oyster::Physics::ICustomBody *obj)
	{
		//Player *player = ((Player*)(rigidBodyPlayer->gameObjectRef));
		//Object *realObj = (Object*)obj->gameObjectRef;

		//switch (realObj->GetType())
		//{
		//case OBJECT_TYPE::OBJECT_TYPE_BOX:
		//	PlayerVBox(*player,(*(DynamicObject*) realObj));
		//	break;
		//case OBJECT_TYPE::OBJECT_TYPE_PLAYER:
		//
		//	break;
		//}

		return Physics::ICustomBody::SubscriptMessage_none;
	}
		
	void PlayerVBox(Player &player, DynamicObject &box)
	{
		player.DamageLife(20);
	}
		
	Physics::ICustomBody::SubscriptMessage CollisionManager::BoxCollision(const Oyster::Physics::ICustomBody *rigidBodyBox, const Oyster::Physics::ICustomBody *obj)
	{
		if(rigidBodyBox == 0)
		{
			return Physics::ICustomBody::SubscriptMessage::SubscriptMessage_none;
		}
		//DynamicObject *box = (DynamicObject*)rigidBodyBox->gameObjectRef;
		//Object *realObj = (Object*)obj->gameObjectRef;

		//switch (realObj->GetType())
		//{
		//case OBJECT_TYPE::OBJECT_TYPE_BOX:
		//		
		//	break;
		//case OBJECT_TYPE::OBJECT_TYPE_PLAYER:
		//	//PlayerVBox(*(Player*)realObj,*box);
		//	break;
		//}

		return Physics::ICustomBody::SubscriptMessage_none;
	}
