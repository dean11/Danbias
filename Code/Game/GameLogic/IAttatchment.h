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

		virtual void UseAttatchment(const WEAPON_FIRE &usage, float dt) = 0;
		virtual void Update(float dt) = 0;

		virtual Player* GetOwner()							{ return owner; }  
		virtual Oyster::Math::Float GetCurrentVelocity()	{ return currentEnergy ? *currentEnergy : 0.0f; } 
		virtual Oyster::Math::Float GetPreviousVelocity()	{ return previousEnergy ? *previousEnergy : 0.0f; } 
		virtual Oyster::Math::Float IsModified()			{ return !((currentEnergy ? *currentEnergy : 0.0f) == (previousEnergy ? *previousEnergy : 0.0f)); } 

	private:

	protected:
		Player *owner;
		Oyster::Math::Float* currentEnergy;
		Oyster::Math::Float* previousEnergy;
	};
}
#endif
