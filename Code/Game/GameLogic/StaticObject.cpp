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
	//use setMass(when it is made) and set the mass to 0 in order to ensure that the object is static
}

StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	//use setMass(when it is made) and set the mass to 0 in order to ensure that the object is static
}

StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision)
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	//use setMass(when it is made) and set the mass to 0 in order to ensure that the object is static
}

StaticObject::StaticObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision)
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	//use setMass(when it is made) and set the mass to 0 in order to ensure that the object is static
}
StaticObject::~StaticObject(void)
{

}
