/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\GameLobby.h"

using namespace DanBias;
using namespace GameLogic;
using namespace Oyster::Network;


void GameLobby::ParseProtocol(Oyster::Network::CustomNetProtocol& p, NetworkClient* c)
{
	switch (p[0].value.netShort)
	{
		case protocol_General_Status:			this->GeneralStatus			(Protocol_General_Status			(p), c);
		break;
		case protocol_General_Text:				this->GeneralText			(Protocol_General_Text				(p), c);
		break;
		case protocol_Lobby_StartGame:			this->LobbyStartGame		(Protocol_LobbyStartGame			(p), c);
		break;
		case protocol_Lobby_JoinGame:			this->LobbyJoin				(Protocol_LobbyJoinGame				(p), c);
		break;
		case protocol_Lobby_Refresh:			this->LobbyRefresh			(Protocol_LobbyRefresh				(p), c);
		break;
		case protocol_Lobby_GameData:			this->LobbyGameData			(Protocol_LobbyGameData				(p), c);
		break;
		case protocol_Lobby_ClientData:			this->LobbyMainData			(Protocol_LobbyClientData			(p), c);
		break;
		case protocol_Lobby_ClientReadyState:	this->LobbyReady			(Protocol_LobbyClientReadyState		(p), c);
		break;
		case protocol_Lobby_QuerryGameType:		this->LobbyQuerryGameData	(Protocol_QuerryGameType			(), c);
		break;
		case protocol_Lobby_Join:				this->LobbyJoin				(Protocol_LobbyJoin					(p), c);
		break;
	}
}


void GameLobby::GeneralStatus(GameLogic::Protocol_General_Status& p, Oyster::Network::NetworkClient* c)
{
	switch (p.status)
	{
		case Protocol_General_Status::States_ready:
		{
			int temp = FindClient(c);
			if(temp != -1 )
			{
				switch (this->gClients[temp]->GetState())
				{
					case GameClient::ClientState_CreatingGame:
					{
						this->gameSession.Join(this->gClients[temp]);
						this->gClients[temp] = 0;
					}
					break;
				}
			}
			else
			{
				c->Disconnect();
			}
		}
		break;
		case Protocol_General_Status::States_idle:
		{

		}
		break;
		case Protocol_General_Status::States_leave:
		break;
		case Protocol_General_Status::States_disconected:
		{
			Detach(c)->Disconnect();
		}
		break;
	}
}
void GameLobby::GeneralText(GameLogic::Protocol_General_Text& p, Oyster::Network::NetworkClient* c)
{
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(this->gClients[i])
		{
			this->gClients[i]->GetClient()->Send(p);
		}
	}
	printf(p.text.c_str());
}
void GameLobby::LobbyStartGame(GameLogic::Protocol_LobbyStartGame& p, Oyster::Network::NetworkClient* c)
{
	if(this->sessionOwner->GetClient()->GetID() == c->GetID())
	{
		//Send countdown timer before lobby shuts down
		for (unsigned int i = 0; i < this->gClients.Size(); i++)
		{
			this->gClients[i]->GetClient()->Send(Protocol_LobbyStartGame(3.0f));
		}
	}
	else
	{
		//Someone else tried to start the server..
	}
}
void GameLobby::LobbyJoin(GameLogic::Protocol_LobbyJoinGame& p, Oyster::Network::NetworkClient* c)
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
void GameLobby::LobbyQuerryGameData(GameLogic::Protocol_QuerryGameType& p, Oyster::Network::NetworkClient* c)
{
	if(this->gameSession)
	{
		int temp = FindClient(c);

		//Something is wrong
		if(temp == -1)
		{
			c->Disconnect();
		}
		else
		{
			//Send game data
			Protocol_LobbyCreateGame lcg((char)1, (char)0, Utility::String::WStringToString(this->description.mapName, std::string()));
			c->Send(lcg);
			this->gClients[temp]->SetState(GameClient::ClientState_CreatingGame);
		}
	}
	else
	{
		// Nothing.-
	}
}
void GameLobby::LobbyJoin(GameLogic::Protocol_LobbyJoin& p, Oyster::Network::NetworkClient* c)
{
	int i = this->FindClient(c);
	if(i != -1)
	{
		this->gClients[i]->SetAlias(Utility::String::StringToWstring(p.alias, std::wstring()));
	}
	else
	{
		// Nothing._.
	}
}

int GameLobby::FindClient(Oyster::Network::NetworkClient* c)
{
	int temp = -1;
	
	//find client in waiting list
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(this->gClients[i] && this->gClients[i]->GetClient()->GetID() == c->GetID())
		{
			temp = i;
			break;
		}
	}

	return temp;
}