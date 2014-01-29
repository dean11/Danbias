#include "StaticObject.h"

using namespace GameLogic;



StaticObject::StaticObject()
	:Object()
{
	
}
StaticObject::StaticObject(OBJECT_TYPE type)
	:Object(type)
{

}
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type)
	:Object(rigidBody,type)
{

}

StaticObject::StaticObject(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
	:Object(collisionFuncBefore,collisionFuncAfter,type)
{
	
}
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
	:Object(rigidBody, collisionFuncBefore, collisionFuncAfter, type)
{
	
}
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
	:Object(rigidBody, collisionFuncBefore, collisionFuncAfter, type)
{

}

StaticObject::~StaticObject(void)
{

}
