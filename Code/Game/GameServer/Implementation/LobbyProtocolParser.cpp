#include "..\GameLobby.h"

using namespace DanBias;

void GameLobby::ParseLobbyProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::NetworkSession* c)
{
	switch (p[0].value.netShort)
	{
		case protocol_Lobby_Start:
			LobbyStartGame(GameLogic::Protocol_LobbyStartGame(p), c);
		break;

		case protocol_Lobby_Refresh:
			LobbyRefresh(GameLogic::Protocol_LobbyRefresh(p), c);
		break;
		case protocol_Lobby_Login:
		{ 
			LobbyLogin(GameLogic::Protocol_LobbyLogin(p), c);
		} break;
		case protocol_Lobby_Join:
		{
			LobbyJoin(GameLogic::Protocol_LobbyJoin(p), c);
		} break;
	}
}

void GameLobby::LobbyStartGame(GameLogic::Protocol_LobbyStartGame& p, DanBias::NetworkSession* c)
{

}

void GameLobby::LobbyRefresh(GameLogic::Protocol_LobbyRefresh& p, DanBias::NetworkSession* c)
{
	double now = this->timer.getElapsedSeconds() + c->lastPoll;

	if(now > this->refreshFrequency)
	{
		c->lastPoll = (float)now;
	}
}

void GameLobby::LobbyLogin(GameLogic::Protocol_LobbyLogin& p, DanBias::NetworkSession* c)
{

}

void GameLobby::LobbyJoin(GameLogic::Protocol_LobbyJoin& p, DanBias::NetworkSession* c)
{
	for (unsigned int i = 0; i < this->gameLobby.Size(); i++)
	{
		if (this->gameLobby[i]->GetID() == p.value)
		{
			this->gameLobby[i]->Attach(Detach(c));
			return;
		}
	}
}

