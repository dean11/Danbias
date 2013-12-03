#include "CollisionManager.h"

using namespace Oyster;

namespace GameLogic
{

	namespace CollisionManager
	{

		Physics::ICustomBody::SubscriptMessage PlayerCollision(const Oyster::Physics::ICustomBody *rigidBodyPlayer, const Oyster::Physics::ICustomBody *obj)
		{
			Player *player = ((Player*)GameLogic::RefManager::getInstance()->GetMap(*rigidBodyPlayer));
			Object *realObj = GameLogic::RefManager::getInstance()->GetMap(*obj);

			switch (realObj->GetType())
			{
			case Object::OBJECT_TYPE_BOX:
				PlayerVBox(*player,(*(DynamicObject*) realObj));
				break;
			case Object::OBJECT_TYPE_PLAYER:

				break;
			}

			//spela ljud? ta skada? etc etc
			return Physics::ICustomBody::SubscriptMessage_none;
		}
		
		void PlayerVBox(Player &player, DynamicObject &box)
		{
			//spela ljud? ta skada? etc etc
		}
		
		Physics::ICustomBody::SubscriptMessage BoxCollision(const Oyster::Physics::ICustomBody *rigidBodyBox, const Oyster::Physics::ICustomBody *obj)
		{
			DynamicObject *box = ((DynamicObject*)GameLogic::RefManager::getInstance()->GetMap(*rigidBodyBox));
			Object *realObj = GameLogic::RefManager::getInstance()->GetMap(*obj);

			switch (realObj->GetType())
			{
			case Object::OBJECT_TYPE_BOX:
				
				break;
			case Object::OBJECT_TYPE_PLAYER:
				PlayerVBox(*(Player*)realObj,*box);
				break;
			}

			return Physics::ICustomBody::SubscriptMessage_none;
		}
	}
}