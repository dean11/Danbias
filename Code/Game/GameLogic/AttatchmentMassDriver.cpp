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
	//Oyster::Math::Float4 pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (500 * dt);
	Oyster::Math::Float3 weaponPos;
	weaponPos = owner->GetPosition() + 5 * owner->GetLookDir();
	Oyster::Math::Float4x4 aim = Oyster::Math3D::ViewMatrix_LookAtDirection(owner->GetLookDir(), owner->GetRigidBody()->GetGravityNormal(), weaponPos);
	Oyster::Math::Float4x4 hitSpace = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/4,1,1,20); 
	Oyster::Collision3D::Frustrum hitFrustum = Oyster::Collision3D::Frustrum(Oyster::Math3D::ViewProjectionMatrix(aim,hitSpace));
	
	Oyster::Physics::API::Instance().ApplyEffect(hitFrustum,NULL, ForcePushAction);

}

/********************************************************
* Pulls the player in the direction he is looking, used for fast movement(kinda like a jetpack)
********************************************************/
void AttatchmentMassDriver::ForcePull(const WEAPON_FIRE &usage, float dt)
{
	Oyster::Physics::Struct::CustomBodyState state = this->owner->GetRigidBody()->GetState();

	//do something with state
	state.ApplyLinearImpulse(Oyster::Math::Float3(this->owner->GetLookDir()) * (500 * dt));

	this->owner->GetRigidBody()->SetState(state);
}


