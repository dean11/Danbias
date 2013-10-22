#pragma once

#include <string>

#include "GameState.h"
#include "OysterMath.h"
#include "RenderEngine.h"
#include "MainMenu.h"

class IngameMenu : public GameState
{
public:
	IngameMenu();
	virtual ~IngameMenu();

	void Enter(GameStateStack *stateStack);
	void Exit();

	void Suspend();
	void Resume();

	void Update(float timeDelta);
	void Render() const;

	bool IsTransparent() const;

private:
	GameStateStack *m_stateStack;

	ID3D11ShaderResourceView *m_textures[3];

	Overlay m_backgroundOverlay;
	Overlay m_resumeOverlay;
	Overlay m_disconnectOverlay;
};
