#include "AttatchmentGun.h"
#include "PhysicsAPI.h"
#include "GameLogicStates.h"
#include "Game.h"
using namespace GameLogic;



AttatchmentGun::AttatchmentGun(void)
{
	this->owner = 0;
	this->damage = 0.0f;
	this->Cooldown = 0.0f;
}

AttatchmentGun::AttatchmentGun(Player &owner)
{
	this->owner = &owner;
	this->damage = standardDamage;
	this->Cooldown = standardCooldown;
	this->TimeUntilFire = 0.0f;
}


AttatchmentGun::~AttatchmentGun(void)
{

}

/********************************************************
* Uses the attatchment and will from here switch case the different WEAPON_FIRE's that are to be used
********************************************************/
void AttatchmentGun::UseAttatchment(const GameLogic::WEAPON_FIRE &usage, float dt)
{
	//switch case to determin what functionallity to use in the attatchment
	switch (usage)
	{
	case WEAPON_FIRE::WEAPON_USE_PRIMARY_PRESS:

		if(TimeUntilFire == 0.0f)
		{
			ShootBullet(usage,dt);
			TimeUntilFire = this->Cooldown;
		}

	break;
	}
		
}

void AttatchmentGun::Update(float dt)
{
	if(TimeUntilFire > 0.0f)
	{
		TimeUntilFire-= dt;
	}
	else
	{
		TimeUntilFire = 0.0f;
	}

}

void AttatchmentGun::ShootBullet(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Collision3D::Ray *hitRay;
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float hitDamage = this->damage;

	firedBullet bullet;
	bullet.hitDamage = hitDamage;

	hitRay = new Oyster::Collision3D::Ray(pos,look);

	Oyster::Physics::API::Instance().ApplyEffect(hitRay,&bullet,BulletCollision);

	delete hitRay;
}
