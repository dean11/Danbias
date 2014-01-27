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
		DynamicObject(void* collisionFunc, OBJECT_TYPE type);
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody ,void (*collisionFunc)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type);
		~DynamicObject(void);

	private:

	};

}

#endif