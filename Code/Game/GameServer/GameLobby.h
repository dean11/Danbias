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
#include <LinkedList.h>

namespace DanBias
{
	struct LobbyLevelData
	{
		int maxClients;
		int gameTimeInMinutes;
		std::wstring gameMode;
		std::wstring mapName;
		std::wstring gameName;
		LobbyLevelData()
			:	maxClients(10)
			,	gameTimeInMinutes(10)
			,	gameMode(L"unknown")
			,	mapName(L"unknown")
			,	gameName(L"unknown")
		{ }
	};
	class GameLobby	:public Oyster::Network::NetworkSession
	{
	public:
		GameLobby();
		virtual~GameLobby();
		void Release();
		void Update();

		void SetGameDesc(const LobbyLevelData& desc);
		void GetGameDesc(LobbyLevelData& desc);
		/**
		*	If param is true, the server will start a game session regardless of clients connected.
		*/
		bool StartGameSession( bool forceStart );

		int GetGameSessionClientCount();
		float GetGameTimeLeft() const;

		bool Attach(Utility::DynamicMemory::SmartPointer<GameClient> gClient);

	private:
		void ParseProtocol(Oyster::Network::CustomNetProtocol& p, Oyster::Network::NetworkClient* c);

		void GeneralStatus(GameLogic::Protocol_General_Status& p, Oyster::Network::NetworkClient* c);			//id = protocol_General_Status:
		void GeneralText(GameLogic::Protocol_General_Text& p, Oyster::Network::NetworkClient* c);				//id = protocol_General_Text:
		void LobbyStartGame(GameLogic::Protocol_LobbyStartGame& p, Oyster::Network::NetworkClient* c);			//id = protocol_Lobby_Start:
		void LobbyJoin(GameLogic::Protocol_LobbyJoinGame& p, Oyster::Network::NetworkClient* c);				//id = protocol_Lobby_Login:
		void LobbyRefresh(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c);				//id = protocol_Lobby_Refresh:
		void LobbyGameData(GameLogic::Protocol_LobbyGameData& p, Oyster::Network::NetworkClient* c);			//id = protocol_Lobby_GameData:
		void LobbyMainData(GameLogic::Protocol_LobbyClientData& p, Oyster::Network::NetworkClient* c);			//id = protocol_Lobby_MainData:
		void LobbyReady(GameLogic::Protocol_LobbyClientReadyState& p, Oyster::Network::NetworkClient* c);		//id = protocol_Lobby_ClientReadyState:
		void LobbyQuerryGameData(GameLogic::Protocol_QuerryGameType& p, Oyster::Network::NetworkClient* c);		//id = protocol_Lobby_QuerryGameType:
		void LobbyJoin(GameLogic::Protocol_LobbyJoin& p, Oyster::Network::NetworkClient* c);					//id = protocol_Lobby_Join:

	private:
		int FindClient(Oyster::Network::NetworkClient* c);

	private:
		void ClientEventCallback(Oyster::Network::NetEvent<Oyster::Network::NetworkClient*, Oyster::Network::NetworkClient::ClientEventArgs> e) override;
		void ClientConnectedEvent(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client) override;
		void ProcessClients() override;
		bool Attach(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client) override;

	private:
		//Utility::WinTimer timer;
		//float refreshFrequency;

		Utility::DynamicMemory::LinkedList<Oyster::Network::NetworkClient*> readyList;
		GameSession gameSession;
		LobbyLevelData description;
		Utility::DynamicMemory::SmartPointer<DanBias::GameClient> sessionOwner;
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<GameClient>> gClients;
	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H
