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
	this->energyChange = 0;
	this->rechargeRate = StandardrechargeRate;
	this->force = Standardforce;
}

AttatchmentMassDriver::AttatchmentMassDriver(Player &owner)
{
	this->currentEnergy = StandardMaxEnergy;
	this->maxEnergy = StandardMaxEnergy;
	this->rechargeRate = StandardrechargeRate;
	this->energyChange = 0;
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
	case WEAPON_USE_PRIMARY_PRESS:
		if(currentEnergy >= 9.0f)
		{
			currentEnergy -= 9.0f;
			ForcePush(usage,dt);
			// add CD 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_PrimaryShoot);
		}
		break;

	case WEAPON_USE_SECONDARY_PRESS:
		if(this->hasObject)
		{
			goto CASE_WEAPON_INTERRUPT;
		}
		else if( currentEnergy >= 1.0f )
		{
			
			currentEnergy -= 1.0f;
		
			ForcePull(usage,dt);
			// add CD 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_SecondaryShoot);
		
		}
		break;

	case WEAPON_INTERRUPT:
	CASE_WEAPON_INTERRUPT:
		((DynamicObject*)(this->heldObject->GetCustomTag()))->RemoveManipulation();
		this->hasObject = false;
		this->heldObject = NULL;
		break;

	case WEAPON_USE_SECONDARY_RELEASE:
		{
			if (this->hasObject)	//Dummy check
			{
				//((DynamicObject*)(this->heldObject->GetCustomTag()))->RemoveManipulation();
				//this->hasObject = false;
				//this->heldObject = NULL;
			}
		}
		break;

	case WEAPON_USE_UTILLITY_PRESS:
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
		Oyster::Math::Float3 pos = ownerPos + owner->GetLookDir().GetNormalized()*3;
		heldObject->OverrideGravity(pos, 0);
	
		Oyster::Math::Float3 lookDir = owner->GetLookDir().GetNormalized();
		Oyster::Math::Float3 objDir = heldObject->GetState().centerPos - ownerPos;
		Oyster::Math::Float3 crossTemp = lookDir.Cross(objDir.GetNormalized());
		Oyster::Math::Float3 deltaPos = heldObject->GetState().centerPos - pos;

		crossTemp.Normalize();
		crossTemp = objDir.Cross(crossTemp);
		crossTemp.Normalize();

		Oyster::Math::Float3 backToMeVelocity = 0.0f;

		if(objDir.GetMagnitude() > 3.1f)
		{
			backToMeVelocity = -objDir.GetNormalized()*deltaPos.GetMagnitude();
		}
		else if(objDir.GetMagnitude() < 2.9f)
		{
			backToMeVelocity = objDir.GetNormalized()*deltaPos.GetMagnitude();
		}

		heldObject->SetLinearVelocity(crossTemp*10.0f*deltaPos.GetMagnitude() + backToMeVelocity + this->owner->GetRigidBody()->GetLinearVelocity());

		if(currentEnergy < maxEnergy)
		{
			currentEnergy += rechargeRate * 0.5f; //rechargeRate is halfed if you are holding an object	
			energyChange  += rechargeRate * 0.5f;
		}
		
	}
	else
	{
		if(currentEnergy < maxEnergy)
		{
			currentEnergy += rechargeRate;
			energyChange  += rechargeRate * 0.5f;
		}
	}

	if(currentEnergy > maxEnergy) 
	{
		currentEnergy = maxEnergy;
		energyChange = 6;
	}
	else if(currentEnergy < 0.0f)
	{
		currentEnergy = 0.0f;
		energyChange = 6;
	}
	
	if(energyChange > 5)
	{
		((Game*)&Game::Instance())->onEnergyUpdateFnc( this->owner, currentEnergy);
		energyChange -= 5;
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
		this->heldObject->ApplyImpulse((Oyster::Math::Float3)pushForce);
		((DynamicObject*)(this->heldObject->GetCustomTag()))->RemoveManipulation();
		this->hasObject = false;
		this->heldObject = NULL;
		return;
	}

	Oyster::Math::Float radius = 4;
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float lenght = 20;
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;

	pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->force * 0.9f);

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
	Oyster::Math::Float lenght = 20;
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;

	Oyster::Math::Float4 pullForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->force * 0.3f);

	Oyster::Collision3D::Cone hitCone(lenght,pos,(Oyster::Math::Float4)owner->GetRigidBody()->GetState().quaternion,radius);
	forcePushData args;
	args.pushForce = -pullForce;
	args.p = this->owner;

	Oyster::Physics::API::Instance().ApplyEffect(&hitCone,&args,ForcePushAction);
}

void AttatchmentMassDriver::PickUpObject(const WEAPON_FIRE &usage, float dt)
{
	//DEBUG:
	MessageBeep(MB_ICONINFORMATION);
	Oyster::Math::Float3 pos = owner->GetPosition() + owner->GetLookDir().GetNormalized() * 1.5f;

	//Do ray test first!
	//Oyster::Collision3D::Ray r(pos, owner->GetLookDir());
	//Oyster::Physics::API::Instance().ApplyEffect(&r, this, AttemptPickUp);

	if(this->hasObject) return;

	Oyster::Collision3D::Sphere hitSphere = Oyster::Collision3D::Sphere(pos , 0.5);
	Oyster::Physics::API::Instance().ApplyEffect(&hitSphere,this,AttemptPickUp);

	return;
}
