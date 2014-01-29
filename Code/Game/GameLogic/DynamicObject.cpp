#include "DynamicObject.h"
#include "CollisionManager.h"

using namespace GameLogic;


DynamicObject::DynamicObject()
	:Object()
{

}
DynamicObject::DynamicObject(OBJECT_TYPE type)
	:Object(type)
{

}
DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type)
	:Object(rigidBody,type)
{

}

DynamicObject::DynamicObject(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
	:Object(collisionFuncBefore,collisionFuncAfter,type)
{

}
DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
	:Object(rigidBody, collisionFuncBefore, collisionFuncAfter, type)
{

}
DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
	:Object(rigidBody, collisionFuncBefore, collisionFuncAfter, type)
{

}

DynamicObject::~DynamicObject(void)
{

}