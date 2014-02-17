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

void DefaultClientConnectedNotify(int ID, wchar_t clientAlias[255], wchar_t clientIp[255])
{

}
void DefaultClientDisconnectedNotify(int ID)
{

}
namespace
{
	GameLobby									lobby;
	NetworkServer								server;
	WinTimer									timer;
	GameServerAPI::ClientConnectedNotify		clientConnectedCallback = DefaultClientConnectedNotify;
	GameServerAPI::ClientDisconnectedNotify		clientDisconnectedCallback = DefaultClientDisconnectedNotify;
}



DanBiasServerReturn GameServerAPI::ServerInitiate(const ServerInitDesc& desc)
{
	ServerOptions opt;
	opt.mainOptions.listenPort = desc.listenPort;
	opt.mainOptions.ownerSession = &lobby;
	if(server.Init(opt) == NetworkServer::ServerReturnCode_Error)
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
	if(!server.IsRunning()) return;
	lobby.Release();
	server.Shutdown();
	

	double total = timer.getElapsedSeconds();
	int time = (int)total;
	int hour, min, sec; 

	hour = time / 3600; 
	time = time % 3600; 
	min = time / 60; 
	time = time % 60;
	sec = time; 

	printf( "Server has been running for: %i:%i:%i - [hh:mm:ss] \n\n", hour, min, sec );
	//printf( "Terminating in : ");
	//for (int i = 0; i < 3; i++)
	//{
	//	printf( "%i ", 3-i );
	//	Sleep(1000);
	//}
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
	std::wstring temp;
	Utility::String::StringToWstring(server.GetLanAddress().c_str(), temp) ;
	memcpy(&i.serverIp[0], temp.c_str(), sizeof(wchar_t) * (temp.length() + 1));
	i.listenPort = server.GetPort();
	return i;
}
bool GameServerAPI::ServerIsRunning()
{
	return server.IsRunning();
}
void GameServerAPI::NotifyWhenClientConnect(ClientConnectedNotify func)
{
	if(!func)	clientConnectedCallback = DefaultClientConnectedNotify;
	else		clientConnectedCallback = func;


}
void GameServerAPI::NotifyWhenClientDisconnect(ClientDisconnectedNotify func)
{
	if(!func)	clientDisconnectedCallback = DefaultClientDisconnectedNotify;
	else		clientDisconnectedCallback = func;
}

void		GameServerAPI::GameSetMapName(const wchar_t* val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	//d.mapNumber = val; //TODO: implement
	lobby.SetGameDesc(d);
}
void		GameServerAPI::GameSetMaxClients(const int& val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.maxClients = val;
	lobby.SetGameDesc(d);
}
void		GameServerAPI::GameSetGameMode(const wchar_t* val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	//d.gameMode = val;	//TODO: implement
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
	if(lobby.StartGameSession())
	{
		
		return true;
	}
	return false;
}

