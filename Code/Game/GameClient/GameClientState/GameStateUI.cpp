#include "GameStateUI.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;

GameStateUI::GameStateUI()
{
	this->nextState = GameStateUI::UIState_same;
}

GameStateUI::~GameStateUI() { /* Do nothing */ }

const GameStateUI::NetEvent & GameStateUI::DataRecieved( const GameStateUI::NetEvent &message )
{
	/* Do nothing */
	return message;
}