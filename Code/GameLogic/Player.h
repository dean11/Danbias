#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

namespace GameLogic
{

	class Player : public Object
	{

	public:
		Player(void);
		~Player(void);
	
		void Update();

		void Move();
		void Shoot();

	private:
		//Life
		//weapon
	};

}
#endif