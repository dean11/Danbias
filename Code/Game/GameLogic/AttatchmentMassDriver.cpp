#include "AttatchmentMassDriver.h"
#include "PhysicsAPI.h"
#include "GameLogicStates.h"

using namespace GameLogic;



AttatchmentMassDriver::AttatchmentMassDriver(void)
{
	this->owner = 0;
	this->heldObject = NULL;
	this->hasObject = false;
}

AttatchmentMassDriver::AttatchmentMassDriver(Player &owner)
{
	
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
		ForcePush(usage,dt);
	break;
	case WEAPON_FIRE::WEAPON_USE_SECONDARY_PRESS:
		
		if(hasObject) 
		{
			//ForcePush(usage,dt);//WARNING THIS IS A CRAP TEST TO MAKE SURE YOU CAN SHOOT BOXES
			break;
		}
		ForcePull(usage,dt);
		break;
	case WEAPON_FIRE::WEAPON_USE_UTILLITY_PRESS:
		ForceZip(usage,dt);
		break;
	}
		
}

void AttatchmentMassDriver::Update(float dt)
{
	
	//update position of heldObject if there is an object being held
	if(hasObject)
	{
		Oyster::Physics::ICustomBody::State state;
		state = heldObject->GetState();
		Oyster::Math::Float3 ownerPos = owner->GetPosition();
		Oyster::Physics::ICustomBody::State ownerState =  owner->GetRigidBody()->GetState();
		Oyster::Math::Float3  up = -ownerState.GetOrientation().v[2];
		up *= -0.3;
		Oyster::Math::Float3 pos = ownerPos + up + (owner->GetLookDir().GetNormalized()*5);

		state.centerPos = pos;

		heldObject->SetState(state);
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
		Oyster::Physics::API::Instance().ReleaseFromLimbo(heldObject);
		pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (700);
		Oyster::Physics::ICustomBody::State state = heldObject->GetState();
		//state.ApplyLinearImpulse((Oyster::Math::Float3)pushForce);
		heldObject->SetState(state);
		
		hasObject = false;
		heldObject = NULL;
		return;
	}
	Oyster::Math::Float3 up = owner->GetOrientation().v[1];
	Oyster::Math::Float3 look = owner->GetLookDir();
	Oyster::Math::Float3 pos = owner->GetPosition();

	pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (200 * dt);
	Oyster::Math::Float4x4 aim = Oyster::Math3D::ViewMatrix_LookAtDirection(look, up, pos);

	Oyster::Math::Float4x4 hitSpace = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/8,1,1,50); 
	Oyster::Collision3D::Frustrum hitFrustum = Oyster::Collision3D::Frustrum(Oyster::Math3D::ViewProjectionMatrix(aim,hitSpace));


	Oyster::Collision3D::Cone *hitCone;
	forcePushData args;
	args.pushForce = pushForce;

	//Oyster::Physics::API::Instance().ApplyEffect(hitFrustum,&args,ForcePushAction);
}

/********************************************************
* Pulls the player in the direction he is looking, used for fast movement(kinda like a jetpack)
********************************************************/
void AttatchmentMassDriver::ForceZip(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Physics::Struct::CustomBodyState state = this->owner->GetRigidBody()->GetState();

	//do something with state
	//state.ApplyLinearImpulse(Oyster::Math::Float3(this->owner->GetLookDir()) * (500 * dt));

	this->owner->GetRigidBody()->SetState(state);
}


void AttatchmentMassDriver::ForcePull(const WEAPON_FIRE &usage, float dt)
{
	//if(hasObject) return; //this test checks if the weapon already has something picked up, if so then it cant use this function

	PickUpObject(usage,dt);	//first test if there is a nearby object to pickup

	if(hasObject) return; //this test checks if the weapon has now picked up an object, if so then it shall not apply a force to suck in objects
	

	//if no object has been picked up then suck objects towards you
	Oyster::Math::Float4 pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (100 * dt);
	Oyster::Math::Float4x4 aim = Oyster::Math3D::ViewMatrix_LookAtDirection(owner->GetLookDir(), owner->GetRigidBody()->GetState().GetOrientation().v[2].xyz, owner->GetPosition());

	Oyster::Math::Float4x4 hitSpace = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/4,1,1,20); 
	Oyster::Collision3D::Frustrum hitFrustum = Oyster::Collision3D::Frustrum(Oyster::Math3D::ViewProjectionMatrix(aim,hitSpace));
	forcePushData args;
	args.pushForce = -pushForce;

	//Oyster::Physics::API::Instance().ApplyEffect(hitFrustum,&args,ForcePushAction);
}

void AttatchmentMassDriver::PickUpObject(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float3 pos = owner->GetPosition() + owner->GetLookDir().GetNormalized()*5;
	Oyster::Collision3D::Sphere *hitSphere = new Oyster::Collision3D::Sphere(pos,20);

	Oyster::Physics::API::Instance().ApplyEffect(hitSphere,this,AttemptPickUp);

	delete hitSphere;
}
