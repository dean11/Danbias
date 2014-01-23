
#define NOMINMAX
#include <Windows.h>
#include <vld.h>

#include "DanBiasGame.h"
#include <DanBiasServerAPI.h>

#include <thread>


void ServerFnc()
{
	
	if( DanBias::DanBiasServerAPI::Initiate() == DanBias::DanBiasServerReturn_Sucess)
	{
		DanBias::DanBiasServerAPI::Run();
		DanBias::DanBiasServerAPI::Release();
	}
	Sleep(100);
}
void ClientFnc()
{
	// Game client starter code goes here
	DanBias::DanBiasGameDesc gameDesc;
	gameDesc.port = 15151;
	//gameDesc.port = 15152;
	//gameDesc.IP = "193.11.184.196";
	//gameDesc.IP = "193.11.184.31";
	//gameDesc.IP = "194.47.150.56";
	gameDesc.IP = "127.0.0.1";

	if( DanBias::DanBiasGame::Initiate(gameDesc) == DanBias::DanBiasClientReturn_Sucess)
	{
		DanBias::DanBiasGame::Run();
		DanBias::DanBiasGame::Release();
	}
	Sleep(100);
}


int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	std::thread serverThread;
	std::thread clientThread;

	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}

	serverThread = std::thread(ServerFnc);

	Sleep(200);

	clientThread = std::thread(ClientFnc);

	if (serverThread.joinable())	serverThread.join();
	if (clientThread.joinable())	clientThread.join();

	return cmdShow;
}