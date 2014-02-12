//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H
#include "GameLogicDef.h"
#include "Object.h"


namespace GameLogic
{
	class DynamicObject : public Object
	{

	public:
		DynamicObject();
		DynamicObject(OBJECT_TYPE type);
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type);
		DynamicObject( void* collisionFuncAfter, OBJECT_TYPE type);
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void* collisionFuncAfter, OBJECT_TYPE type);
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type);

		~DynamicObject(void);

	private:

	};

}

#endif