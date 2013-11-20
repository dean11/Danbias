#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Player.h"

namespace GameLogic
{
	class Game
	{
	private:
		
		
	private:
		GameLogic::Level *level;
		GameLogic::Player *player;

	public:
		Game();
		~Game();
		
		void Init();
		void StartGame();
		void Update();
	};
}

	
	

#endif