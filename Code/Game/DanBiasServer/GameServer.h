/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_GAME_SERVER_H
#define DANBIASSERVER_GAME_SERVER_H

#include <vld.h>

#include "Include\DanBiasServerAPI.h"
#include "ServerObjects\Lobby\MainLobby.h"
#include <NetworkServer.h>
#include <NetworkCallbackHelper.h>

namespace DanBias
{
	class GameServer :public Oyster::Network::ClientConnectedObject
	{
	public:
		GameServer();
		~GameServer();

		DanBiasServerReturn Create();
		DanBiasServerReturn Run();
		DanBiasServerReturn Release();

	private:
		//static void ClientConnectCallbackFunction(Oyster::Network::NetworkClient& connectedClient);
		void ClientConnectCallback(Oyster::Network::NetworkClient &client) override;

		bool initiated;
		bool running;
		bool released;
		int maxClients;
		MainLobby *mainLobby;
		Oyster::Network::NetworkServer *server;

	private:
		struct InitData
		{
			int port;
			int clients;
		};
		bool LoadIniFile(InitData&);
	};
}// End namspace DanBias
#endif // !DANBIASSERVER_DBSERVER_H
