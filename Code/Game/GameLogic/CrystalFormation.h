#ifndef CRYSTALFORMATION_H
#define CRYSTALFORMATION_H
#include "StaticObject.h"
namespace GameLogic
{
	class CrystalFormation : public StaticObject
	{
	public:
		CrystalFormation(void);

		CrystalFormation(Oyster::Physics::ICustomBody *rigidBody 
			,void (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss)
			,OBJECT_TYPE type, Oyster::Math::Float shreddingDamage);

		~CrystalFormation(void);

	private:
		Oyster::Math::Float shreddingDamage;
	};
}
#endif