//////////////////////////////////////////////////
// Launcher to launch Danbias server			//
// Created by [Dennis Andersen] [2013]			//
//////////////////////////////////////////////////
#define NOMINMAX	//Blame it on microsoft
#include <Windows.h>
#include <vld.h>
#include <iostream>
#include <WindowShell.h>

#include <GameServerAPI.h>

int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}

	WindowShell::CreateConsoleWindow();

	DanBias::GameServerAPI::ServerInitDesc desc;
	desc.listenPort = 15151;
	if(DanBias::GameServerAPI::ServerInitiate(desc) == DanBias::DanBiasServerReturn_Sucess)
	{

		DanBias::GameServerAPI::ServerStart();
		//DanBias::GameServerAPI::GameStart();
		while (!(GetAsyncKeyState(0x51)))	//Q for exit
		{
			DanBias::GameServerAPI::ServerUpdate();
			Sleep(1);
		}
		DanBias::GameServerAPI::ServerStop();
	}
	return cmdShow;
}