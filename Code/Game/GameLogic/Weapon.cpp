#include "Weapon.h"
#include "AttatchmentSocket.h"
#include "AttatchmentMassDriver.h"

using namespace GameLogic;

struct Weapon::PrivateData
{
	PrivateData()
	{
		weaponState = WEAPON_STATE_IDLE;
		SelectedAttatchment = new AttatchmentMassDriver();
	}

	~PrivateData()
	{
		delete SelectedAttatchment;
	}

	WEAPON_STATE weaponState;

	AttatchmentSocket **attatchmentSockets;
	int nrOfAttatchmentSockets;

	IAttatchment *SelectedAttatchment;

}myData;

Weapon::Weapon()
{
	myData = new PrivateData();
}


Weapon::~Weapon(void)
{
	delete myData;
}

/********************************************************
* Uses the weapon based on the input given and the current chosen attatchment
********************************************************/
void Weapon::UseWeapon(const WEAPON_FIRE &fireInput)
{
	myData->SelectedAttatchment->UseAttatchment(fireInput);
}

/********************************************************
* Specific weapon usage implementation
********************************************************/

/********************************************************
* Get functions for states 
********************************************************/
bool Weapon::IsFireing()
{
	return (myData->weaponState == WEAPON_STATE::WEAPON_STATE_FIREING);
}

bool Weapon::IsIdle()
{
	return (myData->weaponState == WEAPON_STATE::WEAPON_STATE_IDLE);
}

bool Weapon::IsReloading()
{
	return (myData->weaponState == WEAPON_STATE::WEAPON_STATE_RELOADING);
}

