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
	
	enum keyInput
	{
		keyInput_W,
		keyInput_A,
		keyInput_S,
		keyInput_D,
		keyInput_none
	};

	class GAME_DLL_USAGE IGame
	{
	private:
		Game* gameModule;
	public:
		IGame();
		~IGame();


		void Init();
		void StartGame();
		/************************************************************************/
		/* Get key input    to update the player                                */
		/************************************************************************/
		void Update(keyInput keyPressed, float pitch, float yaw);
		void Render();
		Game* getGameModule();
	private:
	};
}
#endif