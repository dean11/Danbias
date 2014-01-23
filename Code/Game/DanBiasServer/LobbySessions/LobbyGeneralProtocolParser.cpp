#include "MainLobby.h"
#include "LobbyClient.h"

using namespace DanBias;
using namespace GameLogic;

void MainLobby::ParseGeneralProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::LobbyClient* c)
{
	switch (p[0].value.netShort)
	{
		case protocol_General_Status:
		{
			GeneralStatus(GameLogic::Protocol_General_Status(p), c);
		} break;
		case protocol_General_Text:
		{
			GameLogic::Protocol_General_Text(p);
		} break;
		case protocol_Lobby_Login:
		{

		} break;
		case protocol_Lobby_Join:
		{
			JoinLobby(GameLogic::Protocol_LobbyJoin(p), c);
		} break;
	}
}

void MainLobby::GeneralStatus(GameLogic::Protocol_General_Status& p, DanBias::LobbyClient* c)
{
	switch (p.status)
	{
		case Protocol_General_Status::States_ready:
		{

		}
		case Protocol_General_Status::States_idle:
		{

		}
		case Protocol_General_Status::States_leave:
		{

		}
		case Protocol_General_Status::States_disconected:
		{
			Detach(c)->Disconnect();
		}
	}
}

void MainLobby::JoinLobby(GameLogic::Protocol_LobbyJoin& p, DanBias::LobbyClient* c)
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

