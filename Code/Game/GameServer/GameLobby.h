/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_MAINLOBBY_H
#define DANBIASSERVER_MAINLOBBY_H

#include <NetworkSession.h>
#include <Protocols.h>
#include <PostBox\IPostBox.h>
#include <WinTimer.h>

namespace DanBias
{
	class GameLobby	:public Oyster::Network::NetworkSession
	{
	public:
		GameLobby();
		virtual~GameLobby();

		void Release();

		void Frame();

		//void ClientEventCallback(NetEvent<Client*, Client::ClientEventArgs> e) override;

	private:
		void ParseEvents();
		void ParseGeneralProtocol(Oyster::Network::CustomNetProtocol& p, Oyster::Network::NetworkClient* c);
		void ParseLobbyProtocol(Oyster::Network::CustomNetProtocol& p, Oyster::Network::NetworkClient* c);

	//Lobby events
		void LobbyStartGame(GameLogic::Protocol_LobbyStartGame& p, Oyster::Network::NetworkClient* c);
		void LobbyRefresh(GameLogic::Protocol_LobbyRefresh& p, Oyster::Network::NetworkClient* c);

	//General events
		void GeneralStatus(GameLogic::Protocol_General_Status& p, Oyster::Network::NetworkClient* c);
		void GeneralText(GameLogic::Protocol_General_Text& p, Oyster::Network::NetworkClient* c);

	private:
		void ClientEventCallback(Oyster::Network::NetEvent<Oyster::Network::NetworkClient*, Oyster::Network::NetworkClient::ClientEventArgs> e) override;
		void ClientConnectedEvent(Oyster::Network::NetEvent<Oyster::Network::NetworkClient*, Oyster::Network::NetworkClient::ClientEventArgs> e) override;

	private:
		Utility::WinTimer timer;
		float refreshFrequency;
	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H
