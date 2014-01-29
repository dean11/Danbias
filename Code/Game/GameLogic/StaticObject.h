//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "Object.h"

namespace GameLogic
{

	class StaticObject : public Object
	{
	
	public:
		StaticObject();
		StaticObject(OBJECT_TYPE type);
		StaticObject(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type);
		StaticObject(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type);
		StaticObject(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type);
		StaticObject(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type);

		~StaticObject(void);

	private:

	};

}

#endif