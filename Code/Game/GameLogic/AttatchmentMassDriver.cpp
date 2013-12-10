#include "AttatchmentMassDriver.h"

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
}


AttatchmentMassDriver::~AttatchmentMassDriver(void)
{
}

/********************************************************
* Uses the attatchment and will from here switch case the different WEAPON_FIRE's that are to be used
********************************************************/
void AttatchmentMassDriver::UseAttatchment(const GameLogic::WEAPON_FIRE &fireInput)
{
	ForcePush(fireInput);
}

/********************************************************
* This is a specific functionallity of the weapon
********************************************************/
void AttatchmentMassDriver::ForcePush(const GameLogic::WEAPON_FIRE &fireInput)
{
	
}


