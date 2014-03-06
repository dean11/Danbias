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
		AttatchmentMassDriver(Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy);
		AttatchmentMassDriver(Player &owner, Oyster::Math::Float* currEnergy, Oyster::Math::Float* preEnergy);
		~AttatchmentMassDriver(void);


		void UseAttatchment(const WEAPON_FIRE &usage, float dt);
		void Update(float dt);

	private:
		/********************************************************
		* Pushes objects and players in a cone in front of the player
		* @param usage: allows switching on different functionality in this specific function
		********************************************************/
		void ForcePush(const WEAPON_FIRE &usage, float dt);

		/********************************************************
		* Pulls the player forward, this is a movement tool
		* @param usage: allows switching on different functionality in this specific function
		********************************************************/
		void ForceZip(const WEAPON_FIRE &usage, float dt);

		/********************************************************
		* Sucks objects towards the player, the player can then pick up an object and throw it as a projectile
		* @param usage: allows switching on different functionality in this specific function
		********************************************************/
		void ForcePull(const WEAPON_FIRE &usage, float dt);

		/********************************************************
		* Sucks objects towards the player, the player can then pick up an object and throw it as a projectile
		* @param usage: allows switching on different functionality in this specific function
		********************************************************/
		void PickUpObject(const WEAPON_FIRE &usage, float dt);


		static void ForcePushAction(Oyster::Physics::ICustomBody *obj, void* args);
		static void AttemptPickUp(Oyster::Physics::ICustomBody *obj, void* args);

		

	private:
		Oyster::Physics::ICustomBody *heldObject;
		bool hasObject;

		Oyster::Math::Float pushForce;
		Oyster::Math::Float pullForce;
		Oyster::Math::Float zipForce;

		Oyster::Math::Float maxEnergy;
		Oyster::Math::Float rechargeRate;
		Oyster::Math::Float currentCooldown;

	};
}
#endif

