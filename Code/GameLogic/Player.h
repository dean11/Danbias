//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Weapon.h"
#include "IGame.h"


namespace GameLogic
{
	

	class Player : public Object
	{

	public:
		Player(void);
		~Player(void);
	
		void Update(keyInput keyPressed);

		void Move();
		void Shoot();
	

	private:
		int life;
		Weapon *weapon;
		

	};

}
#endif