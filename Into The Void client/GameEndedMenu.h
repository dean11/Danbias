#pragma once

#include <string>

#include "GameState.h"
#include "OysterMath.h"
#include "RenderEngine.h"
#include "NetworkConstants.h"
#include "NetworkMiscFunctions.h"
#include "MainMenu.h"




class GameEndedMenu : public GameState
{
public:
	GameEndedMenu(Player *players[], Score* scores, int winningTeam);
	virtual ~GameEndedMenu();

	void Enter(GameStateStack *stateStack);
	void Exit();

	void Suspend();
	void Resume();

	void Update(float timeDelta);
	void Render() const;

private:
	int winner;
	Score scoreboard[PLAYER_MAX_COUNT];
	Player *playerList[PLAYER_MAX_COUNT];
	GameStateStack *m_stateStack;

	ID3D11ShaderResourceView *m_quitTextures[2];

	Overlay m_backgroundOverlay;
	Overlay m_quitOverlay;
};
