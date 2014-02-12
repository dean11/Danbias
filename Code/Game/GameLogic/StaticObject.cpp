#include "StaticObject.h"
#include "CollisionManager.h"

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
	//this->rigidBody->SetGravity(true);
	//this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(CollisionManager::IgnoreCollision));
}

StaticObject::StaticObject( void* collisionFuncAfter, OBJECT_TYPE type)
	:Object(collisionFuncAfter,type)
{
	
}
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , void* collisionFuncAfter, OBJECT_TYPE type)
	:Object(rigidBody, collisionFuncAfter, type)
{
	
}
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
	:Object(rigidBody, collisionFuncAfter, type)
{

}

StaticObject::~StaticObject(void)
{

}
