#include "ExplosiveCrate.h"

using namespace GameLogic;

ExplosiveCrate::ExplosiveCrate(void)
	:DynamicObject()
{
	this->pushForceMagnitude = 0;
	this->ExplosionRadius = 0;
	this->hasExploaded = false;
}

ExplosiveCrate::ExplosiveCrate(Oyster::Physics::ICustomBody *rigidBody,ObjectSpecialType type, int objectID,Oyster::Math::Float extraDamageOnCollision, Oyster::Math::Float pushForceMagnitude, Oyster::Math::Float ExplosionRadius)
	:DynamicObject(rigidBody,ExplosiveCrate::ExplosiveCrateCollision, type, objectID)
{
	this->extraDamageOnCollision = extraDamageOnCollision;
	this->pushForceMagnitude = pushForceMagnitude;
	this->ExplosionRadius = ExplosionRadius;
	this->hasExploaded = false;
}

ExplosiveCrate::~ExplosiveCrate(void)
{

}





