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
	this->timeUntilFire = 0.0f;
	this->currentEnergy = currEnergy;
	this->previousEnergy = previousEnergy;
}

AttatchmentGun::AttatchmentGun(Player &owner, Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy)
{
	this->owner = &owner;
	this->damage = NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryDamage;
	//this->Cooldown = NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryCooldown;
	this->energyCost = NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryCost;
	this->timeUntilFire = 0.0f;
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
	this->timeUntilFire  -= dt;
	//switch case to determin what functionallity to use in the attatchment
	switch (usage)
	{
	case WEAPON_FIRE::WEAPON_USE_PRIMARY_PRESS:
		if( *currentEnergy >= energyCost &&  this->timeUntilFire <= 0.0f)
		{
			ShootBullet(usage,dt);
			this->timeUntilFire = NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryCooldown;
			(*currentEnergy) -= energyCost; 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_GunShoot);
		}
	break;
	}

}

void AttatchmentGun::Update(float dt)
{
	//this->TimeUntilFire += dt;

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
	::Oyster::Physics::ICustomBody::State ownerState; owner->GetRigidBody()->GetState( ownerState );
	::Oyster::Math::Float3 pos; this->owner->GetWeaponMuzzlePosition( pos, ownerState );
	::Oyster::Math::Float3 target = pos + ( this->owner->GetLookDir() * 100.0f );

	::Oyster::Physics::ICustomBody *hitObject = ::Oyster::Physics::API::Instance().RayClosestObjectNotMe( this->owner->GetRigidBody(), pos, target );
	
	if(hitObject != NULL)
	{
		BulletCollision(hitObject);
	}

	((Game*)&Game::Instance())->onBeamEffectFnc( this->owner, pos, target, 0.1f, NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryCooldown * 0.5 );
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
