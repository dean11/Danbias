/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_MAINLOBBY_H
#define DANBIASSERVER_MAINLOBBY_H

#include <NetworkSession.h>
#include <Protocols.h>
#include <PostBox\IPostBox.h>
#include <WinTimer.h>
#include "GameSession.h"

namespace DanBias
{
	class GameLobby	:public Oyster::Network::NetworkSession
	{
	public:
		GameLobby();
		virtual~GameLobby();
		void Release();
		void Update();

		operator bool();

	private:
		void ParseProtocol(Oyster::Network::CustomNetProtocol& p, Oyster::Network::NetworkClient* c);

		void GeneralStatus(GameLogic::Protocol_General_Status& p, Oyster::Network::NetworkClient* c);		//id = protocol_General_Status:
		void GeneralText(GameLogic::Protocol_General_Text& p, Oyster::Network::NetworkClient* c);			//id = protocol_General_Text:
		void LobbyCreateGame(GameLogic::Protocol_LobbyCreateGame& p, Oyster::Network::NetworkClient* c);	//id = protocol_Lobby_Create:
		void LobbyStartGame(GameLogic::Protocol_LobbyStartGame& p, Oyster::Network::NetworkClient* c);		//id = protocol_Lobby_Start:
		void LobbyJoin(GameLogic::Protocol_LobbyJoin& p, Oyster::Network::NetworkClient* c);				//id = protocol_Lobby_Join:				
		void LobbyLogin(GameLogic::Protocol_LobbyLogin& p, Oyster::Network::NetworkClient* c);				//id = protocol_Lobby_Login:
		void LobbyRefresh(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c);			//id = protocol_Lobby_Refresh:
		void LobbyMainData(GameLogic::Protocol_LobbyMainData& p, Oyster::Network::NetworkClient* c);		//id = protocol_Lobby_MainData:
		void LobbyGameData(GameLogic::Protocol_LobbyGameData& p, Oyster::Network::NetworkClient* c);		//id = protocol_Lobby_GameData:

	private:
		void ClientEventCallback(Oyster::Network::NetEvent<Oyster::Network::NetworkClient*, Oyster::Network::NetworkClient::ClientEventArgs> e) override;
		void ClientConnectedEvent(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client) override;

	private:
		Utility::WinTimer timer;
		float refreshFrequency;
		GameSession* gameSession;
	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H
