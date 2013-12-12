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
void AttatchmentMassDriver::UseAttatchment(const GameLogic::WEAPON_FIRE &fireInput)
{
	//switch case to determin what functionallity to use in the attatchment
	switch (fireInput)
	{
	case WEAPON_FIRE::WEAPON_USE_PRIMARY_PRESS:
		ForcePush(fireInput);
	break;
	case WEAPON_FIRE::WEAPON_USE_SECONDARY_PRESS:
		ForcePull(fireInput);
		break;
	}
	
}

/********************************************************
* This is a specific functionallity of the weapon
********************************************************/
void AttatchmentMassDriver::ForcePush(const GameLogic::WEAPON_FIRE &fireInput)
{
	//create coneRigidBody that will then collide with object and push them in the aimed direction
}


void AttatchmentMassDriver::ForcePull(const WEAPON_FIRE &fireInput)
{
	Oyster::Physics::API::Instance().ApplyForceAt(owner->GetRigidBody(), owner->GetRigidBody()->GetCenter(), owner->GetLookDir() * 100);
}


