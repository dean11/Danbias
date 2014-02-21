#include "DynamicObject.h"
#include "CollisionManager.h"

using namespace GameLogic;
using namespace Oyster::Math;


DynamicObject::DynamicObject()
	:Object()
{
	this->isReleased = false;
	this->isActive = true;
}

DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	this->isReleased = false;
	this->isActive = true;
}
DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	this->isReleased = false;
	this->isActive = true;
}

DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	this->isReleased = false;
	this->isActive = true;
}

DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision)
	:Object(rigidBody, EventOnCollision, type, objectID)	
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	this->isReleased = false;
	this->isActive = true;
}
DynamicObject::~DynamicObject(void)
{

}

void DynamicObject::ReleaseDynamicObject()
{
	//TODO: Inactivate the physics object
	if(this->isReleased) return;

	this->isReleased = true;
	this->isActive = false;
	this->lookDirection = Float3::null;
	this->forwardDirection = Float3::null;
	this->scale = Float3::null;
	this->extraDamageOnCollision = 0;

}
bool DynamicObject::IsReleased()
{
	return this->isReleased;
}
bool DynamicObject::IsActive()
{
	return this->isActive;
}
void DynamicObject::Inactivate()
{
	this->isActive = false;
}
void DynamicObject::Activate()
{
	this->isActive = true;
	this->isReleased = false;
}