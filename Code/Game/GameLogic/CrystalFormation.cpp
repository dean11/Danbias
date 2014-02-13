#include "CrystalFormation.h"

using namespace GameLogic;

CrystalFormation::CrystalFormation(void)
	:StaticObject()
{
	this->shreddingDamage = 0;
}

CrystalFormation::CrystalFormation(Oyster::Physics::ICustomBody *rigidBody, void (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type, Oyster::Math::Float shreddingDamage)
	:StaticObject(rigidBody, collisionFuncAfter, type)
{
	this->shreddingDamage = shreddingDamage;
}


CrystalFormation::~CrystalFormation(void)
{
}
