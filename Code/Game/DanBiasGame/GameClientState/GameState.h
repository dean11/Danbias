#pragma once
#include "GameClientState.h"
namespace DanBias
{
namespace Client
{
class GameState : public GameClientState
{
public:
	GameState(void);
	~GameState(void);
	bool Init();
	GameClientState::ClientState Update();
	bool Render();
	bool Release();
};
};
};
