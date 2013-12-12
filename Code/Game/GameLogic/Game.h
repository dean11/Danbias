#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Player.h"
#include "IGame.h"
#include "Camera.h"
#include "DynamicObject.h"

namespace GameLogic
{
	class Game
	{
	public:
		Game();
		~Game();

		void Init();
		void StartGame();
		void Update(keyInput keyPressed, float pitch, float yaw);
		void Render();
		
	private:
		Level*		level;
		DynamicObject* box;
		Player*		player;
		Camera*		camera;
	};
}		
#endif