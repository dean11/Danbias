#include "StaticObject.h"

using namespace GameLogic;



StaticObject::StaticObject()
	:Object()
{
	
}

StaticObject::StaticObject(void* collisionFunc, OBJECT_TYPE type)
	:Object(collisionFunc,type)
{

}
StaticObject::StaticObject(OBJECT_TYPE type)
	:Object(NULL,type)
{
	
}
/*StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody,void* collisionFunc, OBJECT_TYPE type)
	:Object(rigidBody,collisionFunc,type)
{
	
}*/
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody ,void (*collisionFunc)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
	:Object(rigidBody, collisionFunc, type)
{
	
}


StaticObject::~StaticObject(void)
{

}
