#include "StandaloneGameServerCLI.h"
#include <string>
#include < stdio.h >
#include < stdlib.h >
#include < vcclr.h >


using namespace System;
using namespace System::Windows::Interop;
using namespace System::Windows;
using namespace System::Runtime::InteropServices;

void StandaloneGameServerCLI::NewClientConnected(int ID, wchar_t clientAlias[255], wchar_t clientIp[255])
{

}

StandaloneGameServerCLI::StandaloneGameServerCLI()
{
}

StandaloneGameServerCLI::~StandaloneGameServerCLI()
{
}

DanBiasServerReturn StandaloneGameServerCLI::ServerInitiate(ServerInitDesc desc)
{
	DanBias::GameServerAPI::ServerInitDesc d;
	pin_ptr<const wchar_t> wch = PtrToStringChars(desc.mainOptions.serverName);
	std::wstring temp = wch;
	d.serverName = temp.c_str();
	d.listenPort = desc.mainOptions.listenPort;

	DanBias::GameServerAPI::NotifyWhenClientConnect((DanBias::GameServerAPI::ClientConnectedNotify)StandaloneGameServerCLI::NewClientConnected);

	return (DanBiasServerReturn)DanBias::GameServerAPI::ServerInitiate(d);
}

void StandaloneGameServerCLI::ServerStart()
{
	DanBias::GameServerAPI::ServerStart();
}

void StandaloneGameServerCLI::ServerStop()
{
	DanBias::GameServerAPI::ServerStop();
}

void StandaloneGameServerCLI::ServerUpdate()
{
	DanBias::GameServerAPI::ServerUpdate();
}

GameServerInfo StandaloneGameServerCLI::ServerGetInfo()
{
	GameServerInfo info;

	DanBias::GameServerAPI::GameServerInfo i = DanBias::GameServerAPI::ServerGetInfo();
	info.listenPort = i.listenPort;
	info.serverIp = gcnew String(i.serverIp);

	return info;
}

bool StandaloneGameServerCLI::ServerIsRunning()
{
	return DanBias::GameServerAPI::ServerIsRunning();
}

void StandaloneGameServerCLI::GameSetMapName(String^ value)
{
	pin_ptr<const wchar_t> wch = PtrToStringChars(value);
	DanBias::GameServerAPI::GameSetMapName(wch);
}
void StandaloneGameServerCLI::GameSetGameMode(String^ value)
{
	pin_ptr<const wchar_t> wch = PtrToStringChars(value);
	DanBias::GameServerAPI::GameSetGameMode(wch);
}
void StandaloneGameServerCLI::GameSetGameName(String^ value)
{
	pin_ptr<const wchar_t> wch = PtrToStringChars(value);
	DanBias::GameServerAPI::GameSetGameName(wch);
}

void StandaloneGameServerCLI::GameSetMaxClients(const int val)
{
	DanBias::GameServerAPI::GameSetMaxClients(val);
}



void StandaloneGameServerCLI::GameSetGameTime(const int val)
{
	DanBias::GameServerAPI::GameSetGameTime(val);
}

String^  StandaloneGameServerCLI::GameGetMapName()
{
	return gcnew String( DanBias::GameServerAPI::GameGetMapName());
}

int StandaloneGameServerCLI::GameGetMaxClients()
{
	return DanBias::GameServerAPI::GameGetMaxClients();
}

String^ StandaloneGameServerCLI::GameGetGameMode()
{
	return gcnew String( DanBias::GameServerAPI::GameGetGameMode());
}

int StandaloneGameServerCLI::GameGetGameTime()
{
	return DanBias::GameServerAPI::GameGetGameTime();
}

String^ StandaloneGameServerCLI::GameGetGameName()
{
	return gcnew String( DanBias::GameServerAPI::GameGetGameName());
}

bool StandaloneGameServerCLI::GameStart(bool f)
{
	return DanBias::GameServerAPI::GameStart(f);
}
int StandaloneGameServerCLI::GetClientsConnectedCount()
{
	return DanBias::GameServerAPI::GetConnectedClientCount();
}



