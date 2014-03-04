//////////////////////////////////////////////////
//Created by Erik of the GameLogic team
//////////////////////////////////////////////////
#ifndef ATTATCHMENTGUN_H
#define ATTATCHMENTGUN_H
#include "IAttatchment.h"



namespace GameLogic
{
	const Oyster::Math::Float standardDamage = 16.0f;
	const Oyster::Math::Float standardCooldown = 0.2f;
	
	class AttatchmentGun : public IAttatchment
	{
	public:
		AttatchmentGun(void);
		AttatchmentGun(Player &owner);
		~AttatchmentGun(void);


		void UseAttatchment(const WEAPON_FIRE &usage, float dt);
		void Update(float dt);

	private:
		Oyster::Math::Float damage;
		Oyster::Math::Float Cooldown;
		Oyster::Math::Float TimeUntilFire;

	private:
		void ShootBullet(const WEAPON_FIRE &usage, float dt);
		void BulletCollision(Oyster::Physics::ICustomBody *obj);
	
	};
}
#endif

