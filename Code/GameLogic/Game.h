#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Player.h"

namespace GameLogic
{
	class Game
	{
	public:
		Game();
		~Game();

		void Init();
		void StartGame();
		void Update();
		void Render();
		
		
	private:
		Level*		level;
		Player*		player;

		
	};
}		
#endif