/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_CLIENT_OBJECT_H
#define DANBIASSERVER_CLIENT_OBJECT_H

#include <NetworkClient.h>
#include <PostBox\PostBox.h>
#include <GameAPI.h>

namespace DanBias
{
	class GameClient
	{
	public:
		GameClient(Oyster::Network::NetworkClient client, GameLogic::IPlayerData* player);
		virtual~GameClient();

		//void SetCallback(Oyster::Callback::OysterCallback<void, NetworkSession::NetEvent> value);

		GameLogic::IPlayerData* GetPlayer();
		GameLogic::IPlayerData* ReleasePlayer();
		Oyster::Network::NetworkClient* GetClient();
		Oyster::Network::NetworkClient ReleaseClient();
		int GetID() const;

	private:
		GameLogic::IPlayerData* player;
		Oyster::Network::NetworkClient client;
		int id;
	};

}//End namespace DanBias
#endif // !DANBIASSERVER_CLIENT_OBJECT_H
