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
	this->previousEnergy = 0;

	this->maxEnergy = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::MaxEnergy;
	this->rechargeRate = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::RechargeRate;
	this->pullForce = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pullforce;
	this->pushForce = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pushforce;
	this->zipForce =  NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Zipforce ;
}

AttatchmentMassDriver::AttatchmentMassDriver(Player &owner, Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy)
{
	this->currentEnergy = currEnergy;
	this->previousEnergy = preEnergy;
	this->previousEnergy = 0;
	this->owner = &owner;
	this->heldObject = NULL;
	this->hasObject = false;

	this->maxEnergy = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::MaxEnergy;
	this->rechargeRate = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::RechargeRate;
	this->pullForce = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pullforce;
	this->pushForce = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pushforce;
	this->zipForce =  NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Zipforce ;
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
		if((*currentEnergy) >= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::PushCost && currentCooldown >= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::PrimaryCooldown)
		{
			(*currentEnergy) -= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::PushCost;
			ForcePush(usage,dt);
			currentCooldown = 0.0f;
			(*currentEnergy) = 0.0f;
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_PrimaryShoot);
		}
		break;

	case WEAPON_USE_SECONDARY_PRESS:

	if(this->hasObject)
	{
		break;
	}
	else if( (*currentEnergy) >= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::PullCost )
	{
			
		(*currentEnergy) -= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::PullCost;
		
		ForcePull(usage,dt);
		// add CD 
		((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_SecondaryShoot);
		
	}
	break;

	case WEAPON_INTERRUPT:
CASE_WEAPON_INTERRUPT:
		if(heldObject)
		{
			((DynamicObject*)(this->heldObject->GetCustomTag()))->RemoveManipulation();
			this->hasObject = false;
			this->heldObject = NULL;
			break;
		}

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
		if((*currentEnergy) >= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::ZipCost && currentCooldown >= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::UtilityCooldown)
		{
			(*currentEnergy) -= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::ZipCost;
			currentCooldown = 0.0f;
			ForceZip(usage,dt);
			// add CD 
			((Game*)&Game::Instance())->onActionEventFnc(this->owner, WeaponAction::WeaponAction_UtilityActivate);
		}
		break;
	}
}

void AttatchmentMassDriver::Update(float dt)
{
	
	currentCooldown += dt;
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
			backToMeVelocity = -objDir.GetNormalized()*deltaPos.GetMagnitude()*10.0f;
		}
		else if(objDir.GetMagnitude() < 2.9f)
		{
			backToMeVelocity = objDir.GetNormalized()*deltaPos.GetMagnitude()*10.0f;
		}

		heldObject->SetLinearVelocity(crossTemp*10.0f*deltaPos.GetMagnitude() + backToMeVelocity + this->owner->GetRigidBody()->GetLinearVelocity());

		if(objDir.GetMagnitude() > NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::MaxDistanceFromPlayer)
		{
			heldObject->SetLinearVelocity(Oyster::Math::Float3(0.0f));
			((DynamicObject*)(this->heldObject->GetCustomTag()))->RemoveManipulation();
			this->hasObject = false;
			this->heldObject = NULL;	
		}

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
		pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pushforce);
		pushForce += Oyster::Math::Float4(this->owner->GetLookDir()) * (NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pushforce) * (*currentEnergy / maxEnergy);
		this->heldObject->ApplyImpulse((Oyster::Math::Float3)pushForce);
		((DynamicObject*)this->heldObject->GetCustomTag())->SetAffectedBy(*this->owner);
		((DynamicObject*)(this->heldObject->GetCustomTag()))->RemoveManipulation();
		this->hasObject = false;
		this->heldObject = NULL;
		return;
	}

	Oyster::Math::Float radius = 4.0f;
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float lenght = 6.0f;
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;

	pos += look * ((lenght*0.50f));	//Move the cone to start at the player.

	
	Oyster::Collision3D::Cone *hitCone = new Oyster::Collision3D::Cone(lenght,pos,(Oyster::Math::Float4)owner->GetRigidBody()->GetState().quaternion,radius);

	forcePushData args;
	args.pushForce = NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pushforce;
	args.pushForce += NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pushforce * 3 * (*currentEnergy / maxEnergy);
	args.weapon = this;

	Oyster::Physics::API::Instance().ApplyEffect(hitCone,&args,ForcePushAction);

	if(hitCone) delete hitCone;
}

/********************************************************
* Pulls the player in the direction he is looking, used for fast movement(kinda like a jetpack)
********************************************************/
void AttatchmentMassDriver::ForceZip(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float3 force = Oyster::Math::Float4(this->owner->GetLookDir()) * (this->zipForce);
	Oyster::Math::Float3 pos = owner->GetPosition();

	this->owner->GetRigidBody()->ApplyImpulse(force);

	this->owner->Stun(0.5f);
}


void AttatchmentMassDriver::ForcePull(const WEAPON_FIRE &usage, float dt)
{

	if(hasObject) return; //this test checks if the weapon already has something picked up, if so then it cant use this function

	PickUpObject(usage,dt);	//first test if there is a nearby object to pickup

	if(hasObject) return; //this test checks if the weapon has now picked up an object, if so then it shall not apply a force to suck in objects
	

	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos;// + (owner->GetRigidBody()->GetState().GetOrientation()[1] * 2.0f);
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float3 target = pos + (look * NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::MaxPullDistance);


	forcePushData args;
	args.pushForce = -NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::Pullforce;
	args.weapon = this;

	Oyster::Physics::ICustomBody *hitObject = Oyster::Physics::API::Instance().RayClosestObjectNotMe(this->owner->GetRigidBody(),pos,target);
	
	if(hitObject != NULL)
	{
		ForcePushAction(hitObject,&args);
	}
}

void AttatchmentMassDriver::PickUpObject(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float3 pos = owner->GetRigidBody()->GetState().centerPos; // + owner->GetRigidBody()->GetState().GetOrientation()[1] * 2.0f;
	Oyster::Math::Float3 look = owner->GetLookDir().GetNormalized();
	Oyster::Math::Float3 target = pos + (look * 2.5f);

	if(this->hasObject) return;

	Oyster::Physics::ICustomBody *hitObject = Oyster::Physics::API::Instance().RayClosestObjectNotMe(this->owner->GetRigidBody(),pos,target);

	if (hitObject != NULL)
	{
		this->AttemptPickUp(hitObject,this);
	}
	
	currentCooldown = 0.0f;
	return;
}
