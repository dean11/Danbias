#include "AttatchmentGun.h"
#include "PhysicsAPI.h"
#include "GameLogicStates.h"
#include "Player.h"
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
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float3 target = pos + (look * 100);

	Oyster::Physics::ICustomBody *hitObject = Oyster::Physics::API::Instance().RayClosestObjectNotMe(this->owner->GetRigidBody(),pos,target);
	
	if(hitObject != NULL)
	{
		BulletCollision(hitObject);
	}
}


void AttatchmentGun::BulletCollision(Oyster::Physics::ICustomBody *obj)
	{
		Object *realObj = (Object*)obj->GetCustomTag();
		
		if(realObj->GetObjectType() != ObjectSpecialType::ObjectSpecialType_Player)
		{
			return;
		}
		
		((Player*)realObj)->DamageLife(this->damage);
	}
