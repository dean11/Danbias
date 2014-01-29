//////////////////////////////////////////////////
// Launcher to launch Danbias server			//
// Created by [Dennis Andersen] [2013]			//
//////////////////////////////////////////////////
#define NOMINMAX	//Blame it on windows
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

	DanBias::GameServerAPI::GameInitDesc desc;
	desc.listenPort = 15151;
	if(DanBias::GameServerAPI::Create(desc) == DanBias::DanBiasServerReturn_Sucess)
	{
		DanBias::GameServerAPI::Start();
		DanBias::GameServerAPI::Terminate();
	}
	return cmdShow;
}