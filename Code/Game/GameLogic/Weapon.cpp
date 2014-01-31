#include "Weapon.h"
#include "AttatchmentMassDriver.h"
#include "Player.h"


using namespace GameLogic;
using namespace Utility::DynamicMemory;


Weapon::Weapon()
{
	weaponState = WEAPON_STATE_IDLE;
	selectedAttatchment = 0;
	currentNrOfAttatchments = 0;
	selectedSocketID = 0;
	attatchmentSockets = 0;
}

Weapon::Weapon(int MaxNrOfSockets,Player *owner)
{
	attatchmentSockets.Resize(MaxNrOfSockets);
	attatchmentSockets[0] = new AttatchmentSocket();

	weaponState = WEAPON_STATE_IDLE;
	currentNrOfAttatchments = 0;
	selectedAttatchment = 0;

	//give the weapon a massdriver on socket 0
		IAttatchment *mD = new AttatchmentMassDriver(*owner);
		attatchmentSockets[0]->SetAttatchment(mD);
		this->currentNrOfAttatchments = 1;
		SelectAttatchment(0);
	//give the weapon a massdriver on socket 0
}


Weapon::~Weapon(void)
{

}

/********************************************************
* Uses the weapon based on the input given and the current chosen attatchment
********************************************************/
void Weapon::Use(const WEAPON_FIRE &usage, float dt)
{
	if (selectedAttatchment)
	{
		selectedAttatchment->UseAttatchment(usage, dt);
	}
	
}

/********************************************************
* Specific weapon usage implementation
********************************************************/

/********************************************************
* Get functions for states 
********************************************************/
bool Weapon::IsFireing()
{
	return (weaponState == WEAPON_STATE::WEAPON_STATE_FIRING);
}

bool Weapon::IsIdle()
{
	return (weaponState == WEAPON_STATE::WEAPON_STATE_IDLE);
}

bool Weapon::IsReloading()
{
	return (weaponState == WEAPON_STATE::WEAPON_STATE_RELOADING);
}

bool Weapon::IsValidSocket(int socketID)
{
	if(socketID < (int)attatchmentSockets.Size() && socketID >= 0)
	{
		if (attatchmentSockets[socketID]->GetAttatchment() != 0)
		{
			return true;
		}
	}

	return false;
}

int Weapon::GetCurrentSocketID()
{
	return selectedSocketID;
}


void Weapon::AddNewAttatchment(IAttatchment *attatchment, Player *owner)
{
	if(currentNrOfAttatchments < (int)attatchmentSockets.Size())
	{
		attatchmentSockets[currentNrOfAttatchments]->SetAttatchment(attatchment);
		currentNrOfAttatchments++;
	}
}

void Weapon::SwitchAttatchment(IAttatchment *attatchment, int socketID, Player *owner)
{
	if (IsValidSocket(socketID))
	{
		attatchmentSockets[socketID]->SetAttatchment(attatchment);
	}
}

void Weapon::RemoveAttatchment(int socketID)
{
	if (IsValidSocket(socketID))
	{
		attatchmentSockets[socketID]->RemoveAttatchment();
	}
}

void Weapon::SelectAttatchment(int socketID)
{
	if (IsValidSocket(socketID))
	{
		selectedAttatchment = attatchmentSockets[socketID]->GetAttatchment();
		selectedSocketID = socketID;
	}
	
}