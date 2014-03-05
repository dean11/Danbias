#include "AttatchmentMassDriver.h"
#include "PhysicsAPI.h"
#include "GameLogicStates.h"
#include "Game.h"
using namespace GameLogic;



AttatchmentMassDriver::AttatchmentMassDriver(Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy)
{
	this->owner = 0;
	this->heldObject = NULL;
	this->hasObject = false;
	this->currentEnergy = currEnergy;
	this->previousEnergy = preEnergy;
	this->maxEnergy = StandardMaxEnergy;
	this->previousEnergy = 0;
	this->rechargeRate = StandardrechargeRate;
	this->pullForce = StandarPullforce;
	this->pushForce = StandarPushforce;
	this->zipForce =  StandarZipforce ;
}

AttatchmentMassDriver::AttatchmentMassDriver(Player &owner, Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy)
{
	this->currentEnergy = currEnergy;
	this->previousEnergy = preEnergy;
	this->maxEnergy = StandardMaxEnergy;
	this->rechargeRate = StandardrechargeRate;
	this->pullForce = StandarPullforce;
	this->pushForce = StandarPushforce;
	this->zipForce =  StandarZipforce ;
	
	this->owner = &owner;
	this->heldObject = NULL;
	this->hasObject = false;
}


AttatchmentMassDriver::~AttatchmentMassDriver(void)
{
	this->previousEnergy = 0;
	this->currentEnergy = 0;
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
		if((*currentEnergy) >= 9.0f)
		{
			(*currentEnergy) -= 9.0f;
			ForcePush(usage,dt);
			// add CD 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_PrimaryShoot);
		}
		break;

	case WEAPON_USE_SECONDARY_PRESS:
		if(this->hasObject)
		{
			//goto CASE_WEAPON_INTERRUPT;
		}
		else if( (*currentEnergy) >= 1.0f )
		{
			
			(*currentEnergy) -= 18.0f;
		
			ForcePull(usage,dt);
			// add CD 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_SecondaryShoot);
		
		}
		break;

	case WEAPON_INTERRUPT:
	//CASE_WEAPON_INTERRUPT:
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
		if((*currentEnergy) >= 40.0f)
		{
			(*currentEnergy) -= 40.0f;
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

		if((*currentEnergy) < maxEnergy)
		{
			(*currentEnergy) += rechargeRate * 0.5f; //rechargeRate is halfed if you are holding an object	
		}
		
	}
	else
	{
		if((*currentEnergy) < maxEnergy)
		{
			(*currentEnergy) += rechargeRate;
		}
	}

	if((*currentEnergy) > maxEnergy) 
	{
		(*currentEnergy) = maxEnergy;
	}
	else if((*currentEnergy) < 0.0f)
	{
		(*currentEnergy) = 0.0f;
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
		pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->pushForce * 20);
		this->heldObject->ApplyImpulse((Oyster::Math::Float3)pushForce);
		((DynamicObject*)(this->heldObject->GetCustomTag()))->RemoveManipulation();
		this->hasObject = false;
		this->heldObject = NULL;
		return;
	}

	Oyster::Math::Float radius = 4;
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float lenght = 10;
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;

	pos += look * ((lenght*0.50));	//Move the cone to start at the player.

	
	Oyster::Collision3D::Cone *hitCone = new Oyster::Collision3D::Cone(lenght,pos,(Oyster::Math::Float4)owner->GetRigidBody()->GetState().quaternion,radius);

	forcePushData args;
	args.pushForce = this->pushForce;
	args.p = this->owner;

	Oyster::Physics::API::Instance().ApplyEffect(hitCone,&args,ForcePushAction);

	if(hitCone) delete hitCone;
}

/********************************************************
* Pulls the player in the direction he is looking, used for fast movement(kinda like a jetpack)
********************************************************/
void AttatchmentMassDriver::ForceZip(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float3 force = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->zipForce);

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

	pos += look * ((lenght*0.5f) - 1.0f);	//Move the cone to start at the player.

	Oyster::Collision3D::Cone hitCone(lenght,pos,(Oyster::Math::Float4)owner->GetRigidBody()->GetState().quaternion,radius);
	forcePushData args;
	args.pushForce = -this->pullForce;
	args.p = this->owner;

	Oyster::Physics::API::Instance().ApplyEffect(&hitCone,&args,ForcePushAction);
}

void AttatchmentMassDriver::PickUpObject(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float3 pos = owner->GetPosition() + owner->GetLookDir().GetNormalized() * 1.5f;

	if(this->hasObject) return;

	Oyster::Collision3D::Sphere hitSphere = Oyster::Collision3D::Sphere(pos , 0.5);
	Oyster::Physics::API::Instance().ApplyEffect(&hitSphere,this,AttemptPickUp);

	return;
}
