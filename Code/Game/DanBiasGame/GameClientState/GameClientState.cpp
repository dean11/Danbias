#include "GameClientState.h"

using namespace DanBias::Client;
using namespace ::Oyster::Network;

GameClientState::GameClientState(void)
{
}

GameClientState::~GameClientState(void)
{
}

void GameClientState::DataRecieved( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
{ /* do nothing */ }