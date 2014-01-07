//////////////////////////////////////////////////
// Launcher to launch Danbias server or client	//
// Created by [Dennis Andersen] [2013]			//
//////////////////////////////////////////////////
#define NOMINMAX	//Blame it on windows
#include <Windows.h>
#include <vld.h>


#include <DanBiasServerAPI.h>

int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}
	if( DanBias::DanBiasServerAPI::Initiate() == DanBias::DanBiasServerReturn_Sucess)
	{
		DanBias::DanBiasServerAPI::Run();
		DanBias::DanBiasServerAPI::Release();
	}
	return cmdShow;
}