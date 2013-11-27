#include "CollisionManager.h"

using namespace GameLogic;

CollisionManager::CollisionManager(void)
{
	refManager = new RefManager();
}


CollisionManager::~CollisionManager(void)
{
	SAFE_DELETE(refManager);
}

void CollisionManager::ColisionEvent(Oyster::Physics::ICustomBody &obj1, Oyster::Physics::ICustomBody &obj2)
{
	Object *realObj1 = refManager->GetMap(obj1);
	Object *realObj2 = refManager->GetMap(obj2);
	
	
	switch(realObj1->GetType())
	{
	case Object::OBJECT_TYPE_PLAYER:

		if (realObj2->GetType() == Object::OBJECT_TYPE_BOX )
		{
			PlayerVBox(*((Player*)realObj1),*((DynamicObject*)realObj2));
		}

		break;
	case Object::OBJECT_TYPE_BOX:

		if (realObj2->GetType() == Object::OBJECT_TYPE_PLAYER)
		{
			PlayerVBox(*((Player*)realObj2),*((DynamicObject*)realObj1));
		}

		break;
	}

}

void CollisionManager::PlayerVBox(Player &player, DynamicObject &box)
{
	//spela ljud? ta skada? etc etc
}