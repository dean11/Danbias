//////////////////////////////////////////////////
//Created by Erik of the GameLogic team
//////////////////////////////////////////////////
#ifndef ATTATCHMENTGUN_H
#define ATTATCHMENTGUN_H
#include "IAttatchment.h"



namespace GameLogic
{
	const Oyster::Math::Float standardDamage = 10;

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
	private:
		void ShootBullet(const WEAPON_FIRE &usage, float dt);

	};
}
#endif

