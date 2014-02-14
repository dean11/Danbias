#include "CrystalFormation.h"

using namespace GameLogic;

CrystalFormation::CrystalFormation(void)
	:StaticObject()
{
	this->shreddingDamage = 0;
}

CrystalFormation::CrystalFormation(Oyster::Physics::ICustomBody *rigidBody, int objectID,Oyster::Math::Float shreddingDamage)
	:StaticObject(rigidBody, CrystalFormation::DefaultCollisionAfter, ObjectSpecialType::ObjectSpecialType_CrystalFormation, objectID)
{
	this->shreddingDamage = shreddingDamage;
}


CrystalFormation::~CrystalFormation(void)
{
}

Oyster::Math::Float CrystalFormation::getShreddingDamage()
{
	return this->shreddingDamage;
}
