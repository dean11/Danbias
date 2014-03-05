//////////////////////////////////////////////////
//Created by Erik of the GameLogic team
//////////////////////////////////////////////////
#ifndef ATTATCHMENTGUN_H
#define ATTATCHMENTGUN_H
#include "IAttatchment.h"



namespace GameLogic
{
	const Oyster::Math::Float StandardDamage = 18.0f;
	const Oyster::Math::Float StandardCooldown = 0.2f;

	
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
		Oyster::Math::Float Cooldown;
		Oyster::Math::Float TimeUntilFire;

	private:
		void ShootBullet(const WEAPON_FIRE &usage, float dt);
		void BulletCollision(Oyster::Physics::ICustomBody *obj);
	
	};
}
#endif

