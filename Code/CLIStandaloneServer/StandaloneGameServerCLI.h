#ifndef CLISTANDALONESERVER_CPP_Standalone_CLI_H
#define CLISTANDALONESERVER_CPP_Standalone_CLI_H

#include <windows.h>

#include "..\Game\GameServer\GameServerAPI.h"


namespace System { namespace Windows { namespace Interop
{
	public enum class DanBiasServerReturn
	{
		DanBiasServerReturn_Error,
		DanBiasServerReturn_Sucess,
		DanBiasServerReturn_GameNotCreated,
	};

	public value struct ServerInitDesc
	{
		System::String^ serverName;
		int listenPort;
		bool broadcast;			//Not fully implemented!
	
	};

	public value struct GameServerInfo
	{
		unsigned int listenPort;	// If set to 0, the default port 15151 will be used
		System::String^ serverIp;		// This cant be mofidfied..
	};

	public ref class StandaloneGameServerCLI
	{
	public:
		StandaloneGameServerCLI();
		~StandaloneGameServerCLI();

		DanBiasServerReturn ServerInitiate(ServerInitDesc desc);
		void ServerStart();
		void ServerStop();
		void ServerUpdate();
		GameServerInfo ServerGetInfo();
		bool ServerIsRunning();

		void GameSetMapId(const int val);
		void GameSetMaxClients(const int val);
		void GameSetGameMode(const int val);
		void GameSetGameTime(const int val);
		int GameGetMapId();
		int GameGetMaxClients();
		int GameGetGameMode();
		int GameGetGameTime();
		System::String^ GameGetGameName();
		bool GameStart();
	};

} } }
#endif