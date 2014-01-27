#include "DynamicObject.h"
#include "CollisionManager.h"

using namespace GameLogic;


DynamicObject::DynamicObject()
	:Object()
{
	
}

DynamicObject::DynamicObject(void* collisionFunc, OBJECT_TYPE type)
	:Object(collisionFunc, type)
{

}

DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody ,void (*collisionFunc)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
	:Object(rigidBody, collisionFunc, type)
{
	
}


DynamicObject::~DynamicObject(void)
{

}