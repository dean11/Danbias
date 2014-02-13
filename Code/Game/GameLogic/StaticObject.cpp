#include "StaticObject.h"
#include "CollisionManager.h"

using namespace GameLogic;



StaticObject::StaticObject()
	:Object()
{
	
}
StaticObject::StaticObject(ObjectSpecialType type)
	:Object(type)
{

}
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type)
	:Object(rigidBody,type)
{
	//this->rigidBody->SetGravity(true);
	//this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(CollisionManager::IgnoreCollision));
}

StaticObject::StaticObject( void* collisionFuncAfter, ObjectSpecialType type)
	:Object(collisionFuncAfter,type)
{
	
}
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , void* collisionFuncAfter, ObjectSpecialType type)
	:Object(rigidBody, collisionFuncAfter, type)
{
	
}
StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type)
	:Object(rigidBody, collisionFuncAfter, type)
{

}

StaticObject::~StaticObject(void)
{

}
