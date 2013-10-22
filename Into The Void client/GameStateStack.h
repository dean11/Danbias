#pragma once
#ifndef GAMESTATESTACK_H
#define GAMESTATESTACK_H

#include <vector>

class GameState;

class GameStateStack
{
public:
	GameStateStack(GameState *initialState);
	virtual ~GameStateStack();

	void Update(float timeDelta);
	void Render();

	GameState *CurrentState();
	size_t GetStackSize();

	bool isEmpty();

	// The GameStateStack takes ownership of the state when it's pushed on the stack, and will delete it when it's popped.

	// SetState pops all gamestates currently on the stack and then pushes the new state on the stack.
	void SetState(GameState *state);
	void PushState(GameState *state);
	void PopState();

private:
	std::vector<GameState *> m_stack;
};

#endif
