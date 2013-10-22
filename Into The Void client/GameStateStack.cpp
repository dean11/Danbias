#include <cassert>
#include <algorithm>

#include "GameStateStack.h"
#include "GameState.h"

GameStateStack::GameStateStack(GameState *initialState)
{
	PushState(initialState);
}

GameStateStack::~GameStateStack()
{
	GameState *state;

	while(!m_stack.empty())
	{
		state = m_stack.back();
		m_stack.pop_back();

		state->Exit();
		delete state;
	}
}

void GameStateStack::Update(float timeDelta)
{
	if (m_stack.empty())
		return;

	m_stack.back()->Update(timeDelta);

	for(int i = 0; i < std::max((int)m_stack.size() - 1, 0); i++)
	{
		if (m_stack[i]->WantsBackgroundUpdates())
			m_stack[i]->Update(timeDelta);
	}
}

void GameStateStack::Render()
{
	if (m_stack.empty())
		return;

	size_t lastOpaque = m_stack.size() - 1;

	while(lastOpaque > 0 && m_stack[lastOpaque]->IsTransparent())
	{
		lastOpaque--;
	}

	for(size_t i = lastOpaque; i < m_stack.size(); i++)
	{
		m_stack[i]->Render();
	}
}

GameState *GameStateStack::CurrentState()
{
	return m_stack.back();
}

size_t GameStateStack::GetStackSize()
{
	return m_stack.size();
}

bool GameStateStack::isEmpty()
{
	return m_stack.empty();
}

void GameStateStack::SetState(GameState *state)
{
	GameState *oldState;

	while(!m_stack.empty())
	{
		oldState = m_stack.back();
		m_stack.pop_back();

		oldState->Exit();
		delete oldState;
	}

	if (state)
	{
		m_stack.push_back(state);
		state->Enter(this);
	}
}

void GameStateStack::PushState(GameState *state)
{
	if (!m_stack.empty())
		m_stack.back()->Suspend();

	m_stack.push_back(state);
	state->Enter(this);
}

void GameStateStack::PopState()
{
	assert(!m_stack.empty());

	GameState *state = m_stack.back();
	m_stack.pop_back();

	state->Exit();
	delete state;

	if (!m_stack.empty())
		m_stack.back()->Resume();
}
