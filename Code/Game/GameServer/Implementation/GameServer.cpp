/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <vld.h>

#include "..\GameServerAPI.h"
#include "..\GameLobby.h"
#include "..\GameSession.h"

#include <NetworkServer.h>
#include <NetworkClient.h>

#include <WindowShell.h>
#include <Utilities.h>
#include <WinTimer.h>
#include <thread>

using namespace DanBias;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace Utility;

namespace
{
	GameLobby				lobby;
	NetworkServer			server;
	WinTimer				timer;
}



DanBiasServerReturn GameServerAPI::ServerInitiate(const ServerInitDesc& desc)
{
	if(server.Init(desc.listenPort, &lobby) == NetworkServer::ServerReturnCode_Error)
	{
		return DanBiasServerReturn_Error;
	}
	GameSession::GameDescription d;

	std::printf("Server created!\t-\t%s: [%i]\n\n", server.GetLanAddress().c_str(), desc.listenPort);
		
	return DanBiasServerReturn_Sucess;
}
void GameServerAPI::ServerStart()
{
	timer.reset();
	server.Start();

	
}
void GameServerAPI::ServerStop()
{
	if(!server.IsStarted()) return;
	lobby.Release();
	server.Shutdown();
	

	double total = timer.getElapsedSeconds();
	int time = (int)total;
	int hour, min, sec; 

	hour=time / 3600; 
	time=time % 3600; 
	min=time / 60; 
	time=time % 60;
	sec = time; 

	printf( "Server has been running for: %i:%i:%i - [hh:mm:ss] \n\n", hour, min, sec );
	printf( "Terminating in : ");
	for (int i = 0; i < 3; i++)
	{
		printf( "%i ", 3-i );
		Sleep(1000);
	}
	printf( "\nServer terminated!" );
}
void GameServerAPI::ServerUpdate()
{
	server.Update();
	lobby.Update();

}

GameServerAPI::GameServerInfo GameServerAPI::ServerGetInfo()
{
	GameServerAPI::GameServerInfo i;
	i.serverIp = server.GetLanAddress().c_str();
	i.listenPort = server.GetPort();
	return i;
}
void		GameServerAPI::GameSetMapId(const int& val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.mapNumber = val;
	lobby.SetGameDesc(d);
}
void		GameServerAPI::GameSetMaxClients(const int& val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.maxClients = val;
	lobby.SetGameDesc(d);
}
void		GameServerAPI::GameSetGameMode(const int& val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.gameMode = val;
	lobby.SetGameDesc(d);
}
void		GameServerAPI::GameSetGameTime(const int& val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.gameTime = val;
	lobby.SetGameDesc(d);
}
int			GameServerAPI::GameGetMapId()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.mapNumber;
}
int			GameServerAPI::GameGetMaxClients()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.maxClients;
}
int			GameServerAPI::GameGetGameMode()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.gameMode;
}
int			GameServerAPI::GameGetGameTime()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.gameTime;
}
const char*	GameServerAPI::GameGetGameName()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.gameName.c_str();
}
bool		GameServerAPI::GameStart()
{
	return lobby.StartGameSession();
}

