/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_GAME_SERVER_H
#define DANBIASSERVER_GAME_SERVER_H

#include "GameServerAPI.h"
#include "GameLobby.h"
#include <NetworkServer.h>
#include <Utilities.h>

namespace DanBias
{
	class GameServer
	{
	public:
		GameServer();
		virtual~GameServer();

		DanBiasServerReturn Create(const GameServerAPI::GameInitDesc& desc);

	private:
		static void Run(GameServer* owner);
		void Run();
		void Release();

	private:
		int maxClients;
		Utility::DynamicMemory::SmartPointer<GameLobby> lobby;
		Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkServer> server;
	};
}// End namspace DanBias
#endif // !DANBIASSERVER_DBSERVER_H
