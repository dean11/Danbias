#include "Weapon.h"
#include "AttatchmentGun.h"
#include "AttatchmentMassDriver.h"
#include "Player.h"
#include "Game.h"


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
	for(int i = 0; i < MaxNrOfSockets; i++)
	{
		attatchmentSockets[i] = new AttatchmentSocket();
	}
	

	weaponState = WEAPON_STATE_IDLE;
	currentNrOfAttatchments = 0;
	selectedAttatchment = 0;

	//give the weapon a massdriver on socket 0
	IAttatchment *mD = new AttatchmentMassDriver(*owner, &this->currentEnergy, &this->previousEnergy);
	attatchmentSockets[0]->SetAttatchment(mD);
	this->currentNrOfAttatchments = 1;
	SelectAttatchment(0);
	//give the weapon a massdriver on socket 0

	//give the weapon a normal gun on socket 1
	IAttatchment *gun = new AttatchmentGun(*owner, &this->currentEnergy, &this->previousEnergy);
	attatchmentSockets[1]->SetAttatchment(gun);
	this->currentNrOfAttatchments = 2;
	//SelectAttatchment(1);
	//give the weapon a normal gun on socket 1
}


Weapon::~Weapon(void)
{
	for (unsigned int i = 0; i < this->attatchmentSockets.Size(); i++)
	{
		if(this->attatchmentSockets[i])
		{
			delete this->attatchmentSockets[i];
			this->attatchmentSockets[i] = 0;
		}
			
	}
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

void Weapon::Update(float dt)
{
	if(!selectedAttatchment) return;

	selectedAttatchment->Update(dt);

	if(this->selectedAttatchment->IsModified())
	{
		((Game*)&Game::Instance())->onEnergyUpdateFnc( this->selectedAttatchment->GetOwner(), this->currentEnergy);
		this->previousEnergy = this->currentEnergy;
	}
}