/////////////////////////////////////////////////
// Launcher to launch Danbias server or client //
/////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>
#include <vld.h>


#include "DanBiasServerAPI.h"
//#include "DanBiasGame.h"


int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
#if defined(DANBIAS_SERVER)
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}
	if( DanBias::DanBiasServerAPI::Initiate() == DanBias::DanBiasServerReturn_Sucess)
	{
		DanBias::DanBiasServerAPI::Run();
		DanBias::DanBiasServerAPI::Release();
	}
#elif defined(DANBIAS_CLIENT_L)
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}
	// Game client starter code goes here
	DanBias::DanBiasGameDesc gameDesc;
	gameDesc.port = 1;
	gameDesc.hinst = hinst;
	gameDesc.nCmdShow = cmdShow;

	if( DanBias::DanBiasGame::Initiate(gameDesc) == DanBias::DanBiasClientReturn_Sucess)
	{
		DanBias::DanBiasGame::Run();
		DanBias::DanBiasGame::Release();
	}
#endif

	return cmdShow;
}