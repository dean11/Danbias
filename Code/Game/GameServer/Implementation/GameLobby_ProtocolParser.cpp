#include "..\GameLobby.h"

using namespace DanBias;
using namespace GameLogic;
using namespace Oyster::Network;


void GameLobby::ParseProtocol(Oyster::Network::CustomNetProtocol& p, NetworkClient* c)
{
	switch (p[0].value.netShort)
	{
		case protocol_General_Status:			this->GeneralStatus		(Protocol_General_Status			(p), c);
		break;
		case protocol_General_Text:				this->GeneralText		(Protocol_General_Text				(p), c);
		break;
		//case protocol_Lobby_Create:				this->LobbyCreateGame	(Protocol_LobbyCreateGame			(p), c);
		//break;
		case protocol_Lobby_StartGame:	this->LobbyStartGame	(Protocol_LobbyStartGame	(p), c);
		break;
		//case protocol_Lobby_Join:				this->LobbyJoin			(Protocol_LobbyJoin					(p), c);
		//break;
		case protocol_Lobby_Login:		this->LobbyLogin		(Protocol_LobbyJoinGame		(p), c);
		break;
		case protocol_Lobby_Refresh:			this->LobbyRefresh		(Protocol_LobbyRefresh				(p), c);
		break;
		case protocol_Lobby_GameData:			this->LobbyGameData		(Protocol_LobbyGameData				(p), c);
		break;
		case protocol_Lobby_ClientData:			this->LobbyMainData		(Protocol_LobbyClientData			(p), c);
		break;
		case protocol_Lobby_ClientReadyState:	this->LobbyReady		(Protocol_LobbyClientReadyState		(p), c);
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
//void GameLobby::LobbyCreateGame(GameLogic::Protocol_LobbyCreateGame& p, Oyster::Network::NetworkClient* c)
//{
//	
//}
void GameLobby::LobbyStartGame(GameLogic::Protocol_LobbyStartGame& p, Oyster::Network::NetworkClient* c)
{
	if(this->sessionOwner->GetClient()->GetID() == c->GetID())
	{
		
	}
	else
	{
		//Someone else tried to start the server..
	}
}
//void GameLobby::LobbyJoin(GameLogic::Protocol_LobbyJoin& p, Oyster::Network::NetworkClient* c)
//{
//	//for (unsigned int i = 0; i < this->gameLobby.Size(); i++)
//	//{
//	//	if (this->gameLobby[i]->GetID() == p.value)
//	//	{
//	//		this->gameLobby[i]->Attach(Detach(c));
//	//		return;
//	//	}
//	//}
//}
void GameLobby::LobbyLogin(GameLogic::Protocol_LobbyJoinGame& p, Oyster::Network::NetworkClient* c)
{

}
void GameLobby::LobbyRefresh(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c)
{
	//Dont need to handle this on the server...
}
void GameLobby::LobbyGameData(GameLogic::Protocol_LobbyGameData& p, Oyster::Network::NetworkClient* c)
{
	
}
void GameLobby::LobbyMainData(GameLogic::Protocol_LobbyClientData& p, Oyster::Network::NetworkClient* c)
{

}
void GameLobby::LobbyReady(GameLogic::Protocol_LobbyClientReadyState& p, Oyster::Network::NetworkClient* c)
{
	if(p.isReady)
	{
		this->readyList.PushBack(c);
	}
	else
	{
		this->readyList.Remove(c);
	}
}

