#pragma once

#include <string>

#include "GameState.h"
#include "OysterMath.h"
#include "RenderEngine.h"
#include "MainMenu.h"
#include "NetworkMiscFunctions.h"

class Lobby : public GameState
{
public:
	// Lobby takes ownership of the connection.
	Lobby(NetworkConnection *connection);
	virtual ~Lobby();

	void Enter(GameStateStack *stateStack);
	void Exit();

	void Suspend();
	void Resume();

	void Update(float timeDelta);
	void Render() const;
	int nrPlayers;
private:
	GameStateStack *m_stateStack;
	NetworkConnection *m_connection;
	ID3D11ShaderResourceView *m_queueTextures[3];

	Overlay m_queueOverlay;
};
