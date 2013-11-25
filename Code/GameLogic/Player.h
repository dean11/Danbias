#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Weapon.h"


namespace GameLogic
{
	//void ColisionEvent(unsigned int obj1, unsigned int obj2);
	void DestructionEvent(unsigned int obj1, ::Utility::DynamicMemory::UniquePointer<Oyster::Physics::ICustomBody> obj2);
	
	class Player : public Object
	{

	public:
		Player(void);
		~Player(void);
	
		void Update();

		void Move();
		void Shoot();
	

	private:
		int life;
		Weapon *weapon;
		

	};

}
#endif