#include "GameClientState.h"

using namespace DanBias::Client;
using namespace ::Oyster::Network;

const GameClientState::NetEvent GameClientState::event_processed = GameClientState::NetEvent();

GameClientState::GameClientState() {}

GameClientState::~GameClientState() {}

const GameClientState::NetEvent & GameClientState::DataRecieved( const GameClientState::NetEvent &message )
{
	/* do nothing */
	return message;
}
