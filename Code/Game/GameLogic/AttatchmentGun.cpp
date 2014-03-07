#include "AttatchmentGun.h"
#include "PhysicsAPI.h"
#include "GameLogicStates.h"
#include "Player.h"
#include "Game.h"
#include "AttatchmentMassDriver.h"
using namespace GameLogic;



AttatchmentGun::AttatchmentGun(Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy)
{
	this->owner = 0;
	this->damage = 0.0f;
	this->Cooldown = 0.0f;
	this->currentEnergy = currEnergy;
	this->previousEnergy = previousEnergy;
}

AttatchmentGun::AttatchmentGun(Player &owner, Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy)
{
	this->owner = &owner;
	this->damage = NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryDamage;
	this->Cooldown = NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryCooldown;
	this->TimeUntilFire = 0.0f;
	this->currentEnergy = currEnergy;
	this->previousEnergy = previousEnergy;
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
		if(TimeUntilFire > this->Cooldown)
		{
			ShootBullet(usage,dt);
			TimeUntilFire = 0.0f;
		}
	break;
	}

}

void AttatchmentGun::Update(float dt)
{
	this->TimeUntilFire += dt;

	if((*currentEnergy) < NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::MaxEnergy)
	{
		(*currentEnergy) += NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::RechargeRate;
	}

	if((*currentEnergy) > NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::MaxEnergy) 
	{
		(*currentEnergy) = NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::MaxEnergy;
	}
	else if((*currentEnergy) < 0.0f)
	{
		(*currentEnergy) = 0.0f;
	}
}

void AttatchmentGun::ShootBullet(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos + owner->GetRigidBody()->GetState().GetOrientation()[1];
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

		((Player*)realObj)->SetAffectedBy(*this->owner);
	}
