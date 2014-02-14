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
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID);
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID);


		~DynamicObject(void);

	private:

	};

}

#endif