#include "C++StandaloneCLI.h"

CppStandaloneCLI::CppStandaloneCLI()
{
}

CppStandaloneCLI::~CppStandaloneCLI()
{
}

DanBiasServerReturn CppStandaloneCLI::ServerInitiate(ServerInitDesc desc)
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

void CppStandaloneCLI::ServerStart()
{
	DanBias::GameServerAPI::ServerStart();
}

void CppStandaloneCLI::ServerStop()
{
	DanBias::GameServerAPI::ServerStop();
}

void CppStandaloneCLI::ServerUpdate()
{
	DanBias::GameServerAPI::ServerUpdate();
}

GameServerInfo CppStandaloneCLI::ServerGetInfo()
{
	GameServerInfo info;

	DanBias::GameServerAPI::GameServerInfo i = DanBias::GameServerAPI::ServerGetInfo();
	info.listenPort = i.listenPort;
	info.serverIp = gcnew String(i.serverIp);

	return info;
}

bool CppStandaloneCLI::ServerIsRunning()
{
	return DanBias::GameServerAPI::ServerIsRunning();
}

void CppStandaloneCLI::GameSetMapId(const int val)
{
	DanBias::GameServerAPI::GameSetMapId(val);
}

void CppStandaloneCLI::GameSetMaxClients(const int val)
{
	DanBias::GameServerAPI::GameSetMaxClients(val);
}

void CppStandaloneCLI::GameSetGameMode(const int val)
{
	DanBias::GameServerAPI::GameSetGameMode(val);
}

void CppStandaloneCLI::GameSetGameTime(const int val)
{
	DanBias::GameServerAPI::GameSetGameTime(val);
}

int CppStandaloneCLI::GameGetMapId()
{
	return DanBias::GameServerAPI::GameGetMapId();
}

int CppStandaloneCLI::GameGetMaxClients()
{
	return DanBias::GameServerAPI::GameGetMaxClients();
}

int CppStandaloneCLI::GameGetGameMode()
{
	return DanBias::GameServerAPI::GameGetGameMode();
}

int CppStandaloneCLI::GameGetGameTime()
{
	return DanBias::GameServerAPI::GameGetGameTime();
}

String^ CppStandaloneCLI::GameGetGameName()
{
	return gcnew String(DanBias::GameServerAPI::GameGetGameName());
}

bool CppStandaloneCLI::GameStart()
{
	return DanBias::GameServerAPI::GameStart();
}