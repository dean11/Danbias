/////////////////////////////////////////////////
// Launcher to launch Danbias server or client //
/////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>

//#define DANBIAS_SERVER
#define DANBIAS_CLIENT


#if defined(DANBIAS_SERVER)
#include "DanBiasServer.h"
#elif defined(DANBIAS_CLIENT)
#include "DanBiasGame.h"
#endif

int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
#if defined(DANBIAS_SERVER)
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}
	// Server starter code goes here
	DanBias::DanBiasServerDesc desc;
	desc.port = 0;
	
	if( DanBias::DanBiasServer::Initiate(desc) == DanBias::DanBiasServerReturn_Sucess)
	{
		DanBias::DanBiasServer::Run();
		DanBias::DanBiasServer::Release();
	}
#elif defined(DANBIAS_CLIENT)
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