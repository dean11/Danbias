#include "StaticObject.h"
#include "CollisionManager.h"

using namespace GameLogic;



StaticObject::StaticObject()
	:Object()
{
	
}

StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	rigidBody->SetMass(0);
}

StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	rigidBody->SetMass(0);
}

StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	rigidBody->SetMass(0);
}

StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	rigidBody->SetMass(0);
}
StaticObject::~StaticObject(void)
{

}
