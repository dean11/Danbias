#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameStateStack.h"

class GameState
{
public:
	virtual ~GameState() {}

	virtual void Enter(GameStateStack *stateStack) = 0;
	virtual void Exit() = 0;

	virtual void Suspend() = 0;
	virtual void Resume() = 0;

	virtual void Update(float timeDelta) = 0;
	virtual void Render() const = 0;

	// If this function returns true, the GameState will recieve updates even while it's suspended.
	virtual bool WantsBackgroundUpdates() const
	{
		return false;
	}

	virtual bool IsTransparent() const
	{
		return false;
	}
};

#endif
