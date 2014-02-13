/////////////////////////////////////////////////
// Launcher to launch Danbias server or client //
/////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>
#include <vld.h>

#include "DanBiasGame.h"


int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}
	// Game client starter code goes here
	DanBias::DanBiasGameDesc gameDesc;
	gameDesc.port = 15151;
	//gameDesc.port = 15152;
	//gameDesc.IP = "193.11.184.109";
	//gameDesc.IP = "193.11.184.31";
	//gameDesc.IP = "194.47.150.56";
	gameDesc.IP = "127.0.0.1";
	gameDesc.hinst = hinst;
	gameDesc.nCmdShow = cmdShow;

	if( DanBias::DanBiasGame::Initiate(gameDesc) == DanBias::DanBiasClientReturn_Success )
	{
		DanBias::DanBiasGame::Run();
		DanBias::DanBiasGame::Release();
	}

	return 0;
}