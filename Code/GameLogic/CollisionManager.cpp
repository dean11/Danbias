#include "CollisionManager.h"



namespace GameLogic
{

	namespace CollisionManager
	{

		void PlayerCollision(Oyster::Physics::ICustomBody &rigidBodyPlayer,Oyster::Physics::ICustomBody &obj)
		{
			Player *player = ((Player*)GameLogic::RefManager::getInstance()->GetMap(rigidBodyPlayer));
			Object *realObj = GameLogic::RefManager::getInstance()->GetMap(obj);

			switch (realObj->GetType())
			{
			case Object::OBJECT_TYPE_BOX:
				PlayerVBox(*player,(*(DynamicObject*) realObj));
				break;
			case Object::OBJECT_TYPE_PLAYER:

				break;
			}

			//spela ljud? ta skada? etc etc
		}
		
		void PlayerVBox(Player &player, DynamicObject &box)
		{
			//spela ljud? ta skada? etc etc
		}
		
		void BoxCollision(Oyster::Physics::ICustomBody &rigidBodyBox, Oyster::Physics::ICustomBody &obj)
		{
			DynamicObject *box = ((DynamicObject*)GameLogic::RefManager::getInstance()->GetMap(rigidBodyBox));
			Object *realObj = GameLogic::RefManager::getInstance()->GetMap(obj);

			switch (realObj->GetType())
			{
			case Object::OBJECT_TYPE_BOX:
				
				break;
			case Object::OBJECT_TYPE_PLAYER:
				PlayerVBox(*(Player*)realObj,*box);
				break;
			}
		}
	}
}