#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Player.h"
#include "IGame.h"

namespace GameLogic
{
	

	class Game
	{
	public:
		Game();
		~Game();

		void Init();
		void StartGame();
		void Update(keyInput keyPressed);
		void Render();
		
		
	private:
		Level*		level;
		Player*		player;

		
	};
}		
#endif