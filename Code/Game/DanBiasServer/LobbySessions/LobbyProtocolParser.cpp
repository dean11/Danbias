#include "MainLobby.h"

using namespace DanBias;

void MainLobby::ParseLobbyProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::LobbyClient* c)
{
	switch (p[0].value.netShort)
	{
		case protocol_Lobby_Create:
			CreateGame(GameLogic::Protocol_LobbyCreateGame(p), c);
		break;

		case protocol_Lobby_Start:

		break;

		case protocol_Lobby_Refresh:
			GameLogic::Protocol_LobbyRefresh();
		break;
	}
}

void MainLobby::CreateGame(GameLogic::Protocol_LobbyCreateGame& p, DanBias::LobbyClient* c)
{
	for (unsigned int i = 0; i < this->gameLobby.Size(); i++)
	{
		if(!gameLobby[i])
		{
			gameLobby[i] = new GameLobby(NetworkSession::Detach(c));
			return;
		}
	}

	this->gameLobby.Push(new GameLobby(NetworkSession::Detach(c)));
}


