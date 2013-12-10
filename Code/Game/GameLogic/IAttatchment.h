#ifndef IATTATCHMENT_H
#define IATTATCHMENT_H
#include "GameLogicStates.h"

namespace GameLogic
{
	

	class IAttatchment
	{

	public:

		IAttatchment(void);
		~IAttatchment(void);

		virtual void UseAttatchment(const WEAPON_FIRE &fireInput) = 0;

	private:	
		struct PrivateData;
		PrivateData *myData;

	};
}
#endif
