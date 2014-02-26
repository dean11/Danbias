#include "AttatchmentMassDriver.h"
#include "PhysicsAPI.h"
#include "GameLogicStates.h"
#include "Game.h"
using namespace GameLogic;



AttatchmentMassDriver::AttatchmentMassDriver(void)
{
	this->owner = 0;
	this->heldObject = NULL;
	this->hasObject = false;
	this->currentEnergy = StandardMaxEnergy;
	this->maxEnergy = StandardMaxEnergy;
	this->rechargeRate = StandardrechargeRate;
	this->force = Standardforce;
}

AttatchmentMassDriver::AttatchmentMassDriver(Player &owner)
{
	this->currentEnergy = StandardMaxEnergy;
	this->maxEnergy = StandardMaxEnergy;
	this->rechargeRate = StandardrechargeRate;
	this->force = Standardforce;
	
	this->owner = &owner;
	this->heldObject = NULL;
	this->hasObject = false;
}


AttatchmentMassDriver::~AttatchmentMassDriver(void)
{

}

/********************************************************
* Uses the attatchment and will from here switch case the different WEAPON_FIRE's that are to be used
********************************************************/
void AttatchmentMassDriver::UseAttatchment(const GameLogic::WEAPON_FIRE &usage, float dt)
{
	//switch case to determin what functionallity to use in the attatchment
	switch (usage)
	{
	case WEAPON_FIRE::WEAPON_USE_PRIMARY_PRESS:
		if(currentEnergy >= 90.0f)
		{
			currentEnergy -= 90.0f;
			ForcePush(usage,dt);
			// add CD 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_PrimaryShoot);
		}
	break;

	case WEAPON_FIRE::WEAPON_USE_SECONDARY_PRESS:
		if(currentEnergy >= 1.0f)
		{
			currentEnergy -= 1.0f;
			ForcePull(usage,dt);
			// add CD 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_SecondaryShoot);
		}
	break;

	case WEAPON_FIRE::WEAPON_USE_UTILLITY_PRESS:
		if(currentEnergy >= 90.0f)
		{
			currentEnergy -= 90.0f;
			ForceZip(usage,dt);
			// add CD 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_UtilityActivate);
		}
	break;
	}
		
}

void AttatchmentMassDriver::Update(float dt)
{
	
	//update position of heldObject if there is an object being held
	if(hasObject)
	{
		Oyster::Math::Float3 ownerPos = owner->GetPosition();
		Oyster::Physics::ICustomBody::State ownerState =  owner->GetRigidBody()->GetState();
		Oyster::Math::Float3  up = -ownerState.GetOrientation().v[2];
		up *= -0.3f;
		Oyster::Math::Float3 pos = ownerPos + (owner->GetLookDir().GetNormalized()*2);
		heldObject->SetPosition(pos);
		heldObject->SetLinearVelocity(Oyster::Math::Float3::null);

		currentEnergy += rechargeRate * 0.5f; //rechargeRate is halfed if you are holding an object
	}
	else
	{
		currentEnergy += rechargeRate;
	}
	

}

/********************************************************
* Pushes objects in a cone in front of the weapon when fired
*alternativly it puts a large force on the currently held object
********************************************************/
void AttatchmentMassDriver::ForcePush(const GameLogic::WEAPON_FIRE &usage, float dt)
{
	//if the weapon has an object then it is only the object that will be shot away
	Oyster::Math::Float4 pushForce;

	if(hasObject)
	{
		pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->force);
		heldObject->ApplyImpulse((Oyster::Math::Float3)pushForce);
		((DynamicObject*)(heldObject->GetCustomTag()))->RemoveManipulation();
		hasObject = false;
		heldObject = NULL;
		return;
	}

	Oyster::Math::Float radius = 4;
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float lenght = 10;
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;

	pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->force * 0.6f);

	Oyster::Collision3D::Cone *hitCone = new Oyster::Collision3D::Cone(lenght,pos,(Oyster::Math::Float4)owner->GetRigidBody()->GetState().quaternion,radius);

	forcePushData args;
	args.pushForce = pushForce;
	args.p = this->owner;

	Oyster::Physics::API::Instance().ApplyEffect(hitCone,&args,ForcePushAction);

	if(hitCone) delete hitCone;
}

/********************************************************
* Pulls the player in the direction he is looking, used for fast movement(kinda like a jetpack)
********************************************************/
void AttatchmentMassDriver::ForceZip(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float3 force = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->force);

	this->owner->GetRigidBody()->ApplyImpulse(force);
}


void AttatchmentMassDriver::ForcePull(const WEAPON_FIRE &usage, float dt)
{
	if(hasObject) return; //this test checks if the weapon already has something picked up, if so then it cant use this function

	PickUpObject(usage,dt);	//first test if there is a nearby object to pickup

	if(hasObject) return; //this test checks if the weapon has now picked up an object, if so then it shall not apply a force to suck in objects
	

	//if no object has been picked up then suck objects towards you
	Oyster::Math::Float radius = 4;
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float lenght = 10;
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;

	Oyster::Math::Float4 pullForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->force * 0.2);

	Oyster::Collision3D::Cone *hitCone = new Oyster::Collision3D::Cone(lenght,pos,(Oyster::Math::Float4)owner->GetRigidBody()->GetState().quaternion,radius);
	forcePushData args;
	args.pushForce = -pullForce;
	args.p = this->owner;

	Oyster::Physics::API::Instance().ApplyEffect(hitCone,&args,ForcePushAction);

	if(hitCone) delete hitCone;
}

void AttatchmentMassDriver::PickUpObject(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float3 pos = owner->GetPosition() + owner->GetLookDir().GetNormalized()*2;
	Oyster::Collision3D::Sphere *hitSphere = new Oyster::Collision3D::Sphere(pos,10);

	Oyster::Physics::API::Instance().ApplyEffect(hitSphere,this,AttemptPickUp);

	if(hitSphere) delete hitSphere;
}
