/////////////////////////////////////////////////
// Launcher to launch Danbias server or client //
/////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>
#include <vld.h>

#include <GameServerAPI.h>
#include <WindowShell.h>

using namespace DanBias;



int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}

	WindowShell::CreateConsoleWindow();

	GameServerAPI::ServerInitDesc desc;
	desc.listenPort = 15151;
	desc.serverName = L"bla";

	if(GameServerAPI::ServerInitiate(desc) == DanBiasServerReturn_Sucess)
	{
		GameServerAPI::ServerStart();

		GameServerAPI::GameSetGameMode(L"free-for-all");
		GameServerAPI::GameSetGameName(L"DebugServer");
		GameServerAPI::GameSetGameTime(15);
		GameServerAPI::GameSetMapName(L"2ofAll.bias");
		GameServerAPI::GameSetMaxClients(10);
		
		if(GameServerAPI::GameStart(true))
		{
			int Q = 0x51;
			while ( GetAsyncKeyState(Q) == 0) 
			{
				GameServerAPI::ServerUpdate();
			}
		}
		GameServerAPI::ServerStop();
	}
	
	return 0;
}