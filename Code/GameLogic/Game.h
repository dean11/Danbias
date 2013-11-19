#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Player.h"

namespace GameLogic
{
	class Game
	{

	public:
		Game(void);
		~Game(void);

		void Init();
	private:
		Level *level;
		Player *player;


	};
}

	
	

#endif