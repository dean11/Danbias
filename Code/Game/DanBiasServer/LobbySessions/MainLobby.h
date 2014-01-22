/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_MAINLOBBY_H
#define DANBIASSERVER_MAINLOBBY_H

#include "NetworkSession.h"
#include "GameLobby.h"
#include <Protocols.h>
#include <PostBox\IPostBox.h>
#include <WinTimer.h>

namespace DanBias
{
	class MainLobby :public NetworkSession
	{
	public:
		MainLobby();
		virtual~MainLobby();
		void Release();

		void Frame();

		void SetPostbox(Oyster::IPostBox<NetworkSession::NetEvent>* box);
		Oyster::IPostBox<NetworkSession::NetEvent>* GetPostbox();

		void SetRefreshFrequency(float delta);
		float GetRefreshFrequency() const;

	private:
		void ParseEvents();
		void ParseGeneralProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::LobbyClient* c);
		void ParseLobbyProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::LobbyClient* c);

		void GeneralStatus(GameLogic::Protocol_General_Status& p, DanBias::LobbyClient* c);
		void CreateGame(GameLogic::Protocol_LobbyCreateGame& p, DanBias::LobbyClient* c);
		void JoinLobby(GameLogic::Protocol_LobbyJoin& p, DanBias::LobbyClient* c);

	private:
		Oyster::IPostBox<NetworkSession::NetEvent> *box;
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<GameLobby>> gameLobby;
		Utility::WinTimer timer;
		float refreshFrequency;

	private:
		friend class AdminInterface;
	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H
