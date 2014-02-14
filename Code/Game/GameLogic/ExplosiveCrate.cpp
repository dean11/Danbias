#include "ExplosiveCrate.h"

using namespace GameLogic;

ExplosiveCrate::ExplosiveCrate(void)
	:DynamicObject()
{
	this->shreddingDamage = 0;
	this->pushForce = 0;
	this->ExplosionRadius = 0;
}

ExplosiveCrate::ExplosiveCrate(Oyster::Physics::ICustomBody *rigidBody,ObjectSpecialType type, int objectID,Oyster::Math::Float shreddingDamage, Oyster::Math::Float3 pushForce, Oyster::Math::Float ExplosionRadius)
	:DynamicObject(rigidBody,ExplosiveCrate::ExplosiveCrateCollision, type, objectID)
{
	this->shreddingDamage = shreddingDamage;
	this->pushForce = pushForce;
	this->ExplosionRadius = ExplosionRadius;
}

ExplosiveCrate::~ExplosiveCrate(void)
{

}





