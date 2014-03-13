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

#include "..\GameProtocols\GeneralProtocols.h"

#include <NetworkServer.h>
#include <NetworkClient.h>

#include <WindowShell.h>
#include <Utilities.h>
#include <WinTimer.h>
#include <thread>

//For conversion from wstring to string
#include <codecvt>

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


std::string wstring_to_utf8 (const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

DanBiasServerReturn GameServerAPI::ServerInitiate(const ServerInitDesc& desc)
{
	ServerOptions opt;
	opt.mainOptions.listenPort = desc.listenPort;
	opt.mainOptions.ownerSession = &lobby;

	std::string serverName = wstring_to_utf8(desc.serverName);

	GameLogic::Protocol_Broadcast_Test broadcastMessage(opt.mainOptions.listenPort, "127.0.0.1", serverName);

	opt.broadcastOptions.broadcast = true;
	opt.broadcastOptions.broadcastInterval = 1.0f;
	opt.broadcastOptions.broadcastMessage = broadcastMessage.GetProtocol();

	if(server.Init(opt) == NetworkServer::ServerReturnCode_Error)
	{
		return DanBiasServerReturn_Error;
	}
	GameSession::GameDescription d;

	std::printf("Server created!\t-\t%s: [%i]\n\n", server.GetLanAddress().c_str(), desc.listenPort);
	
	GameLogic::Protocol_Broadcast_Test broadcastMessage2(opt.mainOptions.listenPort, server.GetLanAddress(), serverName);
	server.SetBroadcastMessage(broadcastMessage2.GetProtocol());

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

void			GameServerAPI::GameSetMapName(const wchar_t* val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.mapName = val;
	lobby.SetGameDesc(d);
}
void			GameServerAPI::GameSetGameMode(const wchar_t* val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.gameMode = val;
	lobby.SetGameDesc(d);
}
void			GameServerAPI::GameSetGameName(const wchar_t* val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.gameName = val;
	lobby.SetGameDesc(d);
}
void			GameServerAPI::GameSetMaxClients(const int& val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.maxClients = val;
	lobby.SetGameDesc(d);
}
void			GameServerAPI::GameSetGameTime(const int& val)
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	d.gameTimeInMinutes = val;
	lobby.SetGameDesc(d);
}

const wchar_t*	GameServerAPI::GameGetMapName()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.mapName.c_str();
}
int				GameServerAPI::GameGetMaxClients()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.maxClients;
}
const wchar_t*	GameServerAPI::GameGetGameMode()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.gameMode.c_str();
}
int				GameServerAPI::GameGetGameTime()
{
	return (int)lobby.GetGameTimeLeft();
}
const wchar_t*	GameServerAPI::GameGetGameName()
{
	LobbyLevelData d;
	lobby.GetGameDesc(d);
	return d.gameName.c_str();
}
int				GameServerAPI::GetConnectedClientCount()
{
	return lobby.GetGameSessionClientCount();
}

bool			GameServerAPI::GameStart( bool forceStart )
{
	if(lobby.StartGameSession( forceStart ))
	{
		return true;
	}
	return false;
}

