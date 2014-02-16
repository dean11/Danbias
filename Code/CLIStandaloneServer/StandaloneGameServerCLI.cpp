#include "StandaloneGameServerCLI.h"


using namespace System;
using namespace System::Windows::Interop;
using namespace System::Windows;
using namespace System::Runtime::InteropServices;

StandaloneGameServerCLI::StandaloneGameServerCLI()
{
}

StandaloneGameServerCLI::~StandaloneGameServerCLI()
{
}

DanBiasServerReturn StandaloneGameServerCLI::ServerInitiate(ServerInitDesc desc)
{
	DanBias::GameServerAPI::ServerInitDesc d;
	//Convert from String to char*
	IntPtr p = Marshal::StringToHGlobalAnsi(desc.serverName);
	d.serverName = static_cast<char*>(p.ToPointer());
	Marshal::FreeHGlobal(p);

	d.listenPort = desc.listenPort;
	d.broadcast = desc.broadcast;

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

void StandaloneGameServerCLI::GameSetMapId(const int val)
{
	DanBias::GameServerAPI::GameSetMapId(val);
}

void StandaloneGameServerCLI::GameSetMaxClients(const int val)
{
	DanBias::GameServerAPI::GameSetMaxClients(val);
}

void StandaloneGameServerCLI::GameSetGameMode(const int val)
{
	DanBias::GameServerAPI::GameSetGameMode(val);
}

void StandaloneGameServerCLI::GameSetGameTime(const int val)
{
	DanBias::GameServerAPI::GameSetGameTime(val);
}

int StandaloneGameServerCLI::GameGetMapId()
{
	return DanBias::GameServerAPI::GameGetMapId();
}

int StandaloneGameServerCLI::GameGetMaxClients()
{
	return DanBias::GameServerAPI::GameGetMaxClients();
}

int StandaloneGameServerCLI::GameGetGameMode()
{
	return DanBias::GameServerAPI::GameGetGameMode();
}

int StandaloneGameServerCLI::GameGetGameTime()
{
	return DanBias::GameServerAPI::GameGetGameTime();
}

String^ StandaloneGameServerCLI::GameGetGameName()
{
	return gcnew String(DanBias::GameServerAPI::GameGetGameName());
}

bool StandaloneGameServerCLI::GameStart()
{
	return DanBias::GameServerAPI::GameStart();
}