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
#include <Thread\OysterThread.h>

using namespace DanBias;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace Utility;

namespace
{
	GameLobby				lobby;
	NetworkServer			server;
	WinTimer				timer;
	GameServerAPI			instance;
	//typedef void(*WorkerThreadFnc)(GameServerAPI*);
}


DanBiasServerReturn GameServerAPI::Create(const GameInitDesc& desc)
{
		
	if(server.Init(desc.listenPort, &lobby) == NetworkServer::ServerReturnCode_Error)
	{
		return DanBiasServerReturn_Error;
	}

	std::printf("Server created!\t-\t%s: [%i]\n", server.GetLanAddress().c_str(), desc.listenPort);
		
	return DanBiasServerReturn_Sucess;
}
void GameServerAPI::Start()
{
	server.Start();

	timer.reset();

	while (true)
	{
		int c = server.ProcessConnectedClients();
		if(c > 0)	printf(" - [%i] client(s) connected!\n", c);
		lobby.Update();

		if(GetAsyncKeyState(0x51))	//Q for exit
			break;
	}

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
	for (int i = 0; i < 4; i++)
	{
		printf( "%i ", 3-i );
		Sleep(1000);
	}
	
}
void GameServerAPI::Stop()
{
	server.Stop();
	lobby.ProcessClients();
}
void GameServerAPI::Terminate()
{
	lobby.Release();
	server.Shutdown();

	printf( "Server terminated!" );

}
