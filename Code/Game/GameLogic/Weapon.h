//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef WEAPON_H
#define WEAPON_H
#include "GameLogicStates.h"
#include "IAttatchment.h"
#include "Player.h"
#include "AttatchmentSocket.h"
#include "DynamicArray.h"

namespace GameLogic
{

	class Weapon
	{
	public:
		Weapon(void);
		Weapon(int nrOfAttatchmentSockets, Player *owner);
		~Weapon(void);

		void Use(const WEAPON_FIRE &usage, float dt);
		void Update(float dt);

		void AddNewAttatchment(IAttatchment *attatchment, Player *owner);
		void SwitchAttatchment(IAttatchment *attatchment, int socketID, Player *owner);
		void RemoveAttatchment(int socketID);

		void SelectAttatchment(int socketID);

		bool IsFireing();
		bool IsIdle();
		bool IsReloading();
		bool IsValidSocket(int socketID);

		int GetCurrentSocketID();

	private:
		WEAPON_STATE weaponState;
		Utility::DynamicMemory::DynamicArray<AttatchmentSocket*> attatchmentSockets;
		int currentNrOfAttatchments;
		IAttatchment *selectedAttatchment;
		int selectedSocketID;
		Oyster::Math::Float currentEnergy;
		Oyster::Math::Float previousEnergy;
	};

}
#endif