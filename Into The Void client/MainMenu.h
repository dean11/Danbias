#pragma once

#include <string>

#include "GameState.h"
#include "OysterMath.h"
#include "RenderEngine.h"

struct Overlay
{
	ID3D11ShaderResourceView *texture;
	Oyster::Math::Float3 position;
	Oyster::Math::Float2 size;
};

bool IntersectsOverlay(const Overlay &overlay, Oyster::Math::Float2 point);


class MainMenu : public GameState
{
public:
	MainMenu();
	virtual ~MainMenu();

	void Enter(GameStateStack *stateStack);
	void Exit();

	void Suspend();
	void Resume();

	void Update(float timeDelta);
	void Render() const;

private:
	GameStateStack *m_stateStack;
	NetworkConnection *m_connection;
	float m_connectTimer;

	ID3D11ShaderResourceView *m_connectTextures[2];
	ID3D11ShaderResourceView *m_quitTextures[2];

	Overlay m_backgroundOverlay;
	Overlay m_connectOverlay;
	Overlay m_quitOverlay;
};
