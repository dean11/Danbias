//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef WEAPON_H
#define WEAPON_H
#include "GameLogicStates.h"

namespace GameLogic
{

	class Weapon
	{

	public:
		

		Weapon(void);
		~Weapon(void);

		void UseWeapon(const WEAPON_FIRE &fireInput);


		bool IsFireing();
		bool IsIdle();
		bool IsReloading();


	private:	
		struct PrivateData;
		PrivateData *myData;
	};

}
#endif