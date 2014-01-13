/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_GAME_SERVER_H
#define DANBIASSERVER_GAME_SERVER_H

#include "DanBiasServerAPI.h"
#include "LobbySessions\MainLobby.h"
#include <NetworkServer.h>
#include <NetworkCallbackHelper.h>

namespace DanBias
{
	class GameServer :public Oyster::Network::ClientConnectedObject
	{
	public:
		GameServer();
		virtual~GameServer();

		DanBiasServerReturn Create();
		DanBiasServerReturn Run();
		DanBiasServerReturn Release();

		static NetworkSession* MainLobbyInstance();

	private:
		//static void ClientConnectCallbackFunction(Oyster::Network::NetworkClient& connectedClient);
		void NetworkCallback(Oyster::Network::NetworkClient* client) override;

		bool initiated;
		bool running;
		bool released;
		int maxClients;
		MainLobby *mainLobby;
		Oyster::Network::NetworkServer *server;
		static GameServer* instance;

	private:
		struct InitData
		{
			int port;
			int clients;
		};
		bool LoadIniFile(InitData&);

	private:
		friend class AdminInterface;
	};
}// End namspace DanBias
#endif // !DANBIASSERVER_DBSERVER_H
