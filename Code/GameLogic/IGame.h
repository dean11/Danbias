//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////

#ifndef IGAME_H
#define IGAME_H

#if defined GAME_DLL_EXPORT
#define GAME_DLL_USAGE __declspec(dllexport)
#else
#define GAME_DLL_USAGE __declspec(dllimport)
#endif

namespace GameLogic
{
	class Game;

	class GAME_DLL_USAGE IGame
	{
	private:
		Game* gameModule;
	public:
		IGame();
		~IGame();

		void Init();
		void StartGame();
		void Update();
		void Render();
		Game* getGameModule();
	private:
	};
}
#endif