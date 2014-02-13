#include "DynamicObject.h"
#include "CollisionManager.h"

using namespace GameLogic;


DynamicObject::DynamicObject()
	:Object()
{

}
DynamicObject::DynamicObject(ObjectSpecialType type)
	:Object(type)
{

}
DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type)
	:Object(rigidBody,type)
{

}

DynamicObject::DynamicObject( void* collisionFuncAfter, ObjectSpecialType type)
	:Object(collisionFuncAfter,type)
{

}
DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void* collisionFuncAfter, ObjectSpecialType type)
	:Object(rigidBody, collisionFuncAfter, type)
{

}
DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type)
	:Object(rigidBody, collisionFuncAfter, type)
{

}

DynamicObject::~DynamicObject(void)
{

}