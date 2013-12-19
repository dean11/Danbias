#include "Weapon.h"
#include "AttatchmentSocket.h"
#include "AttatchmentMassDriver.h"

using namespace GameLogic;

struct Weapon::PrivateData
{
	PrivateData()
	{
		weaponState = WEAPON_STATE_IDLE;
		selectedAttatchment = 0;
		currentNrOfAttatchments = 0;
		selectedSocketID = 0;
		maxNrOfSockets = 0;
		attatchmentSockets = 0;
	}

	~PrivateData()
	{
	}

	WEAPON_STATE weaponState;

	AttatchmentSocket **attatchmentSockets;
	int maxNrOfSockets;
	int currentNrOfAttatchments;

	IAttatchment *selectedAttatchment;
	int selectedSocketID;

}myData;

Weapon::Weapon()
{
	myData = new PrivateData();
}

Weapon::Weapon(int MaxNrOfSockets)
{
	myData = new PrivateData();
	myData->maxNrOfSockets = MaxNrOfSockets;
	myData->attatchmentSockets = new AttatchmentSocket*[MaxNrOfSockets];
	for (int i = 0; i < MaxNrOfSockets; i++)
	{
		myData->attatchmentSockets[i] = new AttatchmentSocket();
	}
}


Weapon::~Weapon(void)
{
	for (int i = 0; i < myData->currentNrOfAttatchments; i++)
	{
		delete myData->attatchmentSockets[i];
	}
	delete[] myData->attatchmentSockets;

	delete myData;
}

/********************************************************
* Uses the weapon based on the input given and the current chosen attatchment
********************************************************/
void Weapon::Use(const WEAPON_FIRE &fireInput)
{
	if (myData->selectedAttatchment)
	{
		myData->selectedAttatchment->UseAttatchment(fireInput);
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

bool Weapon::IsValidSocket(int socketID)
{
	if(socketID < myData->maxNrOfSockets && socketID >= 0)
	{
		if (myData->attatchmentSockets[socketID]->GetAttatchment() != 0)
		{
			return true;
		}
	}

	return false;
}

int Weapon::GetCurrentSocketID()
{
	return myData->selectedSocketID;
}


void Weapon::AddNewAttatchment(IAttatchment *attatchment, Player *owner)
{
	if(myData->currentNrOfAttatchments < myData->maxNrOfSockets)
	{
		myData->attatchmentSockets[myData->currentNrOfAttatchments]->SetAttatchment(attatchment);
		myData->currentNrOfAttatchments++;
	}
}

void Weapon::SwitchAttatchment(IAttatchment *attatchment, int socketID, Player *owner)
{
	if (IsValidSocket(socketID))
	{
		myData->attatchmentSockets[socketID]->SetAttatchment(attatchment);
	}
}

void Weapon::RemoveAttatchment(int socketID)
{
	if (IsValidSocket(socketID))
	{
		myData->attatchmentSockets[socketID]->RemoveAttatchment();
	}
}

void Weapon::SelectAttatchment(int socketID)
{
	if (IsValidSocket(socketID))
	{
		myData->selectedAttatchment = myData->attatchmentSockets[socketID]->GetAttatchment();
		myData->selectedSocketID = socketID;
	}
	
}