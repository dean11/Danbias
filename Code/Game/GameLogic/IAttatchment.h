//////////////////////////////////////////////////
//Created by Erik of the GameLogic team
//////////////////////////////////////////////////

#ifndef IATTATCHMENT_H
#define IATTATCHMENT_H
#include "GameLogicStates.h"
#include "Player.h"

namespace GameLogic
{
	

	class IAttatchment
	{

	public:

		IAttatchment(void);
		~IAttatchment(void);

		virtual void UseAttatchment(const WEAPON_FIRE &usage) = 0;

	private:

	protected:
		Player *owner;

	};
}
#endif
