//////////////////////////////////////////////////
//Created by Erik of the GameLogic team
//////////////////////////////////////////////////
#ifndef ATTATCHMENTMASSDRIVER_H
#define ATTATCHMENTMASSDRIVER_H
#include "IAttatchment.h"
namespace GameLogic
{

	class AttatchmentMassDriver : public IAttatchment
	{
	public:
		AttatchmentMassDriver(void);
		AttatchmentMassDriver(Player &owner);
		~AttatchmentMassDriver(void);


		void UseAttatchment(const WEAPON_FIRE &fireInput);

	private:
		/********************************************************
		* Pushes objects and players in a cone in front of the player
		* @param fireInput: allows switching on different functionality in this specific function
		********************************************************/
		void ForcePush(const WEAPON_FIRE &fireInput);

		/********************************************************
		* Pulls the player forward, this is a movement tool
		* @param fireInput: allows switching on different functionality in this specific function
		********************************************************/
		void ForcePull(const WEAPON_FIRE &fireInput);

		/********************************************************
		* Sucks objects towards the player, the player can then pick up an object and throw it as a projectile
		* @param fireInput: allows switching on different functionality in this specific function
		********************************************************/
		void ForceSuck(const WEAPON_FIRE &fireInput);

	private:
		struct PrivateData;
		PrivateData *myData;
	};
}
#endif

