// Game Logic interface 

#ifndef IGAME_H
#define IGAME_H

#if defined DLL_EXPORT
#define DLL_USAGE __declspec(dllexport)
#else
#define DLL_USAGE __declspec(dllimport)
#endif

namespace GameLogic
{
	class Game;

	class DLL_USAGE IGame
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