/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_CLIENT_OBJECT_H
#define DANBIASSERVER_CLIENT_OBJECT_H

#include "..\LobbySessions\LobbyClient.h"
#include <PostBox\PostBox.h>
#include <Game.h>

namespace DanBias
{
	class GameClient: Oyster::Callback::CallbackObject<void, NetworkSession::NetEvent>
	{
	public:
		GameClient(Utility::DynamicMemory::SmartPointer<LobbyClient> client, GameLogic::Game::PlayerData player, Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value);
		virtual~GameClient();

		void SetCallback(Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value);

		//GameLogic::Player* GetPlayer();
		GameLogic::Game::PlayerData* GetPlayer();
		GameLogic::Game::PlayerData ReleasePlayer();

		LobbyClient* GetClient() const;
		Utility::DynamicMemory::SmartPointer<LobbyClient> ReleaseClient();
		int GetID() const;

	private:
		//Utility::DynamicMemory::SmartPointer<GameLogic::Player> player;
		GameLogic::Game::PlayerData player;
		Utility::DynamicMemory::SmartPointer<LobbyClient> client;
		Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> callbackValue;
		int id;
		void ObjectCallback(NetworkSession::NetEvent) override;

	private:
		friend class AdminInterface;
	};

}//End namespace DanBias
#endif // !DANBIASSERVER_CLIENT_OBJECT_H
