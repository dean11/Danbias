#ifndef EXPLOSIVECRATE_H
#define EXPLOSIVECRATE_H
#include "DynamicObject.h"
namespace GameLogic
{
	class ExplosiveCrate : public DynamicObject
	{
	public:
		ExplosiveCrate(void);

		ExplosiveCrate(Oyster::Physics::ICustomBody *rigidBody,ObjectSpecialType type,int objectID,Oyster::Math::Float extraDamageOnCollision, Oyster::Math::Float pushForceMagnitude, Oyster::Math::Float ExplosionRadius);

		~ExplosiveCrate(void);

		static void ExplosiveCrateCollision(Oyster::Physics::ICustomBody *rigidBodyCrate, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);
		static void Explode(Oyster::Physics::ICustomBody *obj, void* args);

	private:
		Oyster::Math::Float pushForceMagnitude;
		Oyster::Math::Float ExplosionRadius;
		bool hasExploaded;
	};


}
#endif