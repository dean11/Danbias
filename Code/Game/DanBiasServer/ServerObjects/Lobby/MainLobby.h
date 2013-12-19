#ifndef DANBIASSERVER_MAINLOBBY_H
#define DANBIASSERVER_MAINLOBBY_H

#include "..\NetworkSession.h"
#include "GameLobby.h"
#include <GameProtocols.h>
#include <PostBox\IPostBox.h>

namespace DanBias
{
	class MainLobby :public NetworkSession
	{
	public:
		MainLobby();
		virtual~MainLobby();
		void Release();

		void Frame();

		Oyster::IPostBox<NetworkSession::NetEvent>* GetPostbox();

	private:
		void ParseEvents();
		void ParseProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::ClientObject& c);

		void CreateGame(GameLogic::Protocol_LobbyCreateGame& p, DanBias::ClientObject& c);
		//void CreateJoin(GameLogic::Protocol_LobbyJoinGame& p, DanBias::ClientObject& c);

		//void ProtocolRecievedCallback(CustomNetProtocol& protocol) override;

	private:
		Oyster::IPostBox<NetworkSession::NetEvent> *box;
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<GameLobby>> gameLobby;
	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H
