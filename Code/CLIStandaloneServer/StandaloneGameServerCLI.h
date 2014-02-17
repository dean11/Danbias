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
		value struct MainOptions
		{
			System::String^ serverName;
			int listenPort;
		} mainOptions;
	};

	public value struct GameServerInfo
	{
		unsigned int listenPort;	// If set to 0, the default port 15151 will be used
		System::String^ serverIp;		// This cant be mofidfied..
	};

	public ref class StandaloneGameServerCLI
	{
	private:
		static void NewClientConnected(int ID, wchar_t clientAlias[255], wchar_t clientIp[255]);

	public:
		StandaloneGameServerCLI();
		~StandaloneGameServerCLI();

		DanBiasServerReturn ServerInitiate(ServerInitDesc desc);
		void ServerStart();
		void ServerStop();
		void ServerUpdate();
		GameServerInfo ServerGetInfo();
		bool ServerIsRunning();

		void GameSetMapName(String^ val);
		void GameSetMaxClients(const int val);
		void GameSetGameMode(String^ val);
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