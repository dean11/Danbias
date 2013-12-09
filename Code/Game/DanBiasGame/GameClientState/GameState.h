#pragma once
#include "GameClientState.h"
namespace DanBias
{
namespace Client
{
class GameState : public GameClientState
{
private:
	struct myData;
	myData* privData;
public:
	GameState(void);
	~GameState(void);
	bool Init();
	GameClientState::ClientState Update(float deltaTime, InputClass* KeyInput);
	bool Render();
	bool Release();
};
};
};
