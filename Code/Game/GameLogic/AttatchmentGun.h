//////////////////////////////////////////////////
//Created by Erik of the GameLogic team
//////////////////////////////////////////////////
#ifndef ATTATCHMENTGUN_H
#define ATTATCHMENTGUN_H
#include "IAttatchment.h"



namespace GameLogic
{
	class AttatchmentGun : public IAttatchment
	{
	public:
		AttatchmentGun(Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy);
		AttatchmentGun(Player &owner, Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy);
		~AttatchmentGun(void);


		void UseAttatchment(const WEAPON_FIRE &usage, float dt);
		void Update(float dt);

	private:
		Oyster::Math::Float damage;
		Oyster::Math::Float timeUntilFire;
		Oyster::Math::Float energyCost;
	

	private:
		void ShootBullet(const WEAPON_FIRE &usage, float dt);
		void BulletCollision(Oyster::Physics::ICustomBody *obj);
	
	};
}
#endif

