#include "..\GameLobby.h"

using namespace DanBias;
using namespace GameLogic;
using namespace Oyster::Network;


void GameLobby::ParseProtocol(Oyster::Network::CustomNetProtocol& p, NetworkClient* c)
{
	switch (p[0].value.netShort)
	{
		//LobbyStartGame(GameLogic::Protocol_LobbyStartGame(p), c);
		//LobbyRefresh(GameLogic::Protocol_LobbyRefresh(p), c);
		//LobbyLogin(GameLogic::Protocol_LobbyLogin(p), c);
		//LobbyJoin(GameLogic::Protocol_LobbyJoin(p), c);
		//GeneralStatus(GameLogic::Protocol_General_Status(p), c);
		//GeneralText(GameLogic::Protocol_General_Text(p), c);

		case protocol_General_Status:
		break;
		case protocol_General_Text:
		break;
		case protocol_Lobby_Create:
		break;
		case protocol_Lobby_Start:
		break;
		case protocol_Lobby_Join:
		break;
		case protocol_Lobby_Login:
		break;
		case protocol_Lobby_Refresh:
		break;
		case protocol_Lobby_MainData:
		break;
		case protocol_Lobby_GameData:
		break;
	}
}


void GameLobby::GeneralStatus(GameLogic::Protocol_General_Status& p, Oyster::Network::NetworkClient* c)
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
		case Protocol_General_Status::States_disconected:
		{
			Detach(c)->Disconnect();
		}
	}
}
void GameLobby::GeneralText(GameLogic::Protocol_General_Text& p, Oyster::Network::NetworkClient* c)
{
	printf(p.text.c_str());
}
void GameLobby::LobbyCreateGame(GameLogic::Protocol_LobbyCreateGame& p, Oyster::Network::NetworkClient* c)
{
	
}
void GameLobby::LobbyStartGame(GameLogic::Protocol_LobbyStartGame& p, Oyster::Network::NetworkClient* c)
{

}
void GameLobby::LobbyJoin(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c)
{
	//for (unsigned int i = 0; i < this->gameLobby.Size(); i++)
	//{
	//	if (this->gameLobby[i]->GetID() == p.value)
	//	{
	//		this->gameLobby[i]->Attach(Detach(c));
	//		return;
	//	}
	//}
}
void GameLobby::LobbyLogin(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c)
{

}
void GameLobby::LobbyRefresh(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c)
{
	//Dont need to handle this on the server...
}
void GameLobby::LobbyMainData(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c)
{

}
void GameLobby::LobbyGameData(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c)
{

}


