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
			,int objectID,Oyster::Math::Float shreddingDamage);

		~CrystalFormation(void);

		Oyster::Math::Float getShreddingDamage();

	private:
		Oyster::Math::Float shreddingDamage;
	};
}
#endif