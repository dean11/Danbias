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
		void ForcePush(const WEAPON_FIRE &fireInput);
		void ForcePull(const WEAPON_FIRE &fireInput);

	private:
		struct PrivateData;
		PrivateData *myData;
	};
}
#endif

