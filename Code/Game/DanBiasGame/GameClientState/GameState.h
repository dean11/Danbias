#ifndef DANBIAS_CLIENT_GAMESTATE_H
#define DANBIAS_CLIENT_GAMESTATE_H
#include "GameClientState.h"
#include "OysterMath.h"
#include <string>
namespace DanBias
{
namespace Client
{
class GameState : public GameClientState
{
	enum gameStateState
	{
		gameStateState_loading,
		gameStateState_playing,
		gameStateState_end,
	};
private:
	
	struct myData;
	myData* privData;
public:
	GameState(void);
	~GameState(void);
	bool Init();
	GameClientState::ClientState Update(float deltaTime, InputClass* KeyInput);
	bool LoadModels(std::wstring mapFile);
	bool InitCamera(Oyster::Math::Float3 startPos);
	gameStateState LoadGame();

	bool Render();
	bool Release();
};
};
};
#endif