#ifndef CLISTANDALONESERVER_CPP_Standalone_CLI_H
#define CLISTANDALONESERVER_CPP_Standalone_CLI_H

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "..\Game\GameServer\GameServerAPI.h"

enum DanBiasServerReturn
{
	DanBiasServerReturn_Error,
	DanBiasServerReturn_Sucess,
	DanBiasServerReturn_GameNotCreated,
};

public ref struct ServerInitDesc
{
	System::String^ serverName;
	int listenPort;
	bool broadcast;			//Not fully implemented!
	ServerInitDesc() 
	{
		serverName = "Game Server";
		listenPort = 15152;
		broadcast = true;
	};
};

public value struct GameServerInfo
{
	unsigned int listenPort;	// If set to 0, the default port 15151 will be used
	System::String^ serverIp;		// This cant be mofidfied..
};

public ref class CppStandaloneCLI
{
public:
	CppStandaloneCLI();
	~CppStandaloneCLI();

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
	String^ GameGetGameName();
	bool GameStart();
};

#endif