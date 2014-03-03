#include "DynamicObject.h"
#include "CollisionManager.h"
#include "Player.h"

using namespace GameLogic;
using namespace Oyster::Math;


DynamicObject::DynamicObject()
	:Object()
{
	this->isReleased = false;
	this->isActive = true;
	this->affectedBy = NULL;
	this->manipulatedBy = NULL;
}

DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	this->isReleased = false;
	this->isActive = true;
	this->affectedBy = NULL;
	this->manipulatedBy = NULL;
}
DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	this->isReleased = false;
	this->isActive = true;
	this->affectedBy = NULL;
	this->manipulatedBy = NULL;
}

DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision)
	:Object(rigidBody, EventOnCollision, type, objectID)
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	this->isReleased = false;
	this->isActive = true;
	this->affectedBy = NULL;
	this->manipulatedBy = NULL;
}

DynamicObject::DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision)
	:Object(rigidBody, EventOnCollision, type, objectID)	
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	this->isReleased = false;
	this->isActive = true;
	this->affectedBy = NULL;
	this->manipulatedBy = NULL;
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

void DynamicObject::SetAffectedBy(Player &player)
{
	this->affectedBy = &player;
}

Player* DynamicObject::getAffectingPlayer()
{
	return this->affectedBy;
}

void DynamicObject::RemoveAffectedBy()
{
	this->affectedBy = NULL;
}

GameLogic::Player* DynamicObject::getManipulatingPlayer()
{
	return this->manipulatedBy;
}

void DynamicObject::SetManipulatingPlayer(GameLogic::Player &player)
{
	this->manipulatedBy = &player;
}

void DynamicObject::RemoveManipulation()
{
	this->manipulatedBy = NULL;
}