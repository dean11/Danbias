#include "AttatchmentMassDriver.h"
#include "PhysicsAPI.h"

using namespace GameLogic;

struct AttatchmentMassDriver::PrivateData
{
	PrivateData()
	{

	}

	~PrivateData()
	{

	}

}myData;


AttatchmentMassDriver::AttatchmentMassDriver(void)
{
	myData = new PrivateData();
	this->owner = 0;
}

AttatchmentMassDriver::AttatchmentMassDriver(Player &owner)
{
	myData = new PrivateData();
	this->owner = &owner;
}


AttatchmentMassDriver::~AttatchmentMassDriver(void)
{
	delete myData;
}

/********************************************************
* Uses the attatchment and will from here switch case the different WEAPON_FIRE's that are to be used
********************************************************/
void AttatchmentMassDriver::UseAttatchment(const GameLogic::WEAPON_FIRE &usage)
{
	//switch case to determin what functionallity to use in the attatchment
	switch (usage)
	{
	case WEAPON_FIRE::WEAPON_USE_PRIMARY_PRESS:
		ForcePush(usage);
	break;
	case WEAPON_FIRE::WEAPON_USE_SECONDARY_PRESS:
		ForcePull(usage);
		break;
	}
	
}

/********************************************************
* Pushes objects in a cone in front of the weapon when fired
********************************************************/
void AttatchmentMassDriver::ForcePush(const GameLogic::WEAPON_FIRE &usage)
{
	//create coneRigidBody that will then collide with object and push them in the aimed direction
}

/********************************************************
* Pulls the player in the direction he is looking, used for fast movement(kinda like a jetpack)
********************************************************/
void AttatchmentMassDriver::ForcePull(const WEAPON_FIRE &usage)
{
	//Oyster::Physics::API::Instance().ApplyForceAt(owner->GetRigidBody(), owner->GetRigidBody()->GetCenter(), owner->GetLookDir() * 100);
}


