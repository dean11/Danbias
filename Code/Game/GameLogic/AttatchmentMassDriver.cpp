#include "AttatchmentMassDriver.h"
#include "PhysicsAPI.h"

using namespace GameLogic;



AttatchmentMassDriver::AttatchmentMassDriver(void)
{
	this->owner = 0;
}

AttatchmentMassDriver::AttatchmentMassDriver(Player &owner)
{
	
	this->owner = &owner;
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
		ForcePull(usage,dt);
		break;
	}
	
}

/********************************************************
* Pushes objects in a cone in front of the weapon when fired
********************************************************/
void AttatchmentMassDriver::ForcePush(const GameLogic::WEAPON_FIRE &usage, float dt)
{
	Oyster::Math::Float4 pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (500 * dt);
	Oyster::Math::Float4x4 aim = Oyster::Math3D::ViewMatrix_LookAtDirection(owner->GetLookDir(), owner->GetRigidBody()->GetGravityNormal(), owner->GetPosition());
	Oyster::Math::Float4x4 hitSpace = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/4,1,1,20); 
	Oyster::Collision3D::Frustrum hitFrustum = Oyster::Collision3D::Frustrum(Oyster::Math3D::ViewProjectionMatrix(aim,hitSpace));
	
	//Oyster::Physics::API::Instance().ApplyEffect(hitFrustum,ForcePushAction);
	//create frustum that will then collide with object and push them in the aimed direction

	//sample with frustum using visitor pattern(needs a function ptr sent with it that idicates what happens when a collision has been made)
}

void AttatchmentMassDriver::ForcePushAction(Oyster::Physics::ICustomBody *obj)
{
	Oyster::Physics::ICustomBody::State state = obj->GetState();
	Oyster::Math::Float4 pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (500);
	state.ApplyLinearImpulse(pushForce);
	obj->SetState(state);
}

/********************************************************
* Pulls the player in the direction he is looking, used for fast movement(kinda like a jetpack)
********************************************************/
void AttatchmentMassDriver::ForcePull(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Physics::Struct::CustomBodyState state = this->owner->GetRigidBody()->GetState();

	//do something with state
	state.ApplyLinearImpulse(Oyster::Math::Float4(this->owner->GetLookDir()) * (500 * dt));

	this->owner->GetRigidBody()->SetState(state);
}


