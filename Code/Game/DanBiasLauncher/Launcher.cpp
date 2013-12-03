/////////////////////////////////////////////////
// Launcher to launch Danbias server or client //
/////////////////////////////////////////////////
#include <Windows.h>

#define DANBIAS_SERVER
//#define DANBIAS_CLIENT


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
	// Game client starter code goes here
	return cmdShow;
#endif

	return cmdShow;
}