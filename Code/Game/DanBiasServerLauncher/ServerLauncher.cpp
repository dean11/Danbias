//////////////////////////////////////////////////
// Launcher to launch Danbias server			//
// Created by [Dennis Andersen] [2013]			//
//////////////////////////////////////////////////
#define NOMINMAX	//Blame it on windows
#include <Windows.h>
#include <vld.h>


#include <GameServerAPI.h>

int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}

	DanBias::GameServerAPI::GameInitDesc desc;
	desc.connectionPort = 15151;
	desc.maxNumberOfClients = 0;
	desc.threaded = false;
	if( !DanBias::GameServerAPI::Create(desc) == DanBias::DanBiasServerReturn_Sucess)
	{

	}
	return cmdShow;
}