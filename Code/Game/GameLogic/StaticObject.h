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

		StaticObject(ObjectSpecialType type);
		StaticObject(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type);
		StaticObject( void* collisionFuncAfter, ObjectSpecialType type);
		StaticObject(Oyster::Physics::ICustomBody *rigidBody , void* collisionFuncAfter, ObjectSpecialType type);
		StaticObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type);
	
		~StaticObject(void);

	private:

	};

}

#endif