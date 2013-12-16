//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef WEAPON_H
#define WEAPON_H
#include "GameLogicStates.h"
#include "IAttatchment.h"
#include "Player.h"

namespace GameLogic
{

	class Weapon
	{

	public:
		

		Weapon(void);
		Weapon(int nrOfAttatchmentSockets);
		~Weapon(void);

		void Use(const WEAPON_FIRE &fireInput);

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
		struct PrivateData;
		PrivateData *myData;
	};

}
#endif