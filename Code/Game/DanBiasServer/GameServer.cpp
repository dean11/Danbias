/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <WindowShell.h>

#include "GameServer.h"
#include "Utilities.h"
#include "ServerInitReader.h"
#include <TEST_PROTOCOLS.h>

namespace DanBias
{
	using namespace Oyster::Network;

	void GameServer::ClientConnectCallback(NetworkClient &client)
	{
		printf("Client connected!\n");
		GameLogic::Protocol_TEST t;
		t.text = "Hello";
		client.Send(t);
	}
	GameServer::GameServer()
		:	initiated(0)
		,	running(0)
		,	released(0)
		,	maxClients(0)
		,	mainLobby(0)
		,	server(0)
	{
	}
	GameServer::~GameServer()
	{

	}
	DanBiasServerReturn GameServer::Create()
	{
		this->server = new NetworkServer();
		this->mainLobby = new MainLobby();
		
		InitData data;
		if(!LoadIniFile(data)) return DanBiasServerReturn_Error;

		NetworkServer::INIT_DESC serverDesc;
		this->maxClients = data.clients;
		serverDesc.port = data.port;
		serverDesc.recvObj = this;
		serverDesc.callbackType = Oyster::Network::NetworkClientCallbackType_Object;

		if(!this->server->Init(serverDesc))								return DanBiasServerReturn_Error;
		if(!WindowShell::CreateConsoleWindow())							return DanBiasServerReturn_Error;
		if(!WindowShell::CreateWin(WindowShell::WINDOW_INIT_DESC()))	return DanBiasServerReturn_Error;

		this->initiated = true;
		return DanBiasServerReturn_Sucess;
	}
	DanBiasServerReturn GameServer::Run()
	{
		if(this->running)			return DanBiasServerReturn_Error;
		if(this->released)			return DanBiasServerReturn_Error;
		if(!this->initiated)		return DanBiasServerReturn_Error;

		if(!this->server->Start())	return DanBiasServerReturn_Error;

		this->running = true;
		while (this->running)
		{
			if(!WindowShell::Frame()) 
				break;
		}

		return DanBiasServerReturn_Sucess;
	}
	DanBiasServerReturn GameServer::Release()
	{
		this->server->Shutdown();
		delete this->server;
		delete this->mainLobby;
		this->released = true;
		return DanBiasServerReturn_Sucess;
	}

	bool GameServer::LoadIniFile(InitData& ini)
	{
		std::ifstream in;
		std::string f = GetInitPath(InitPath_ServerIni);
		in.open(f, std::ios::in);
		if(!in.is_open()) return false;

		std::string buffer;
		while (!in.eof())
		{
			in >> buffer;

			if(buffer == "port")
			{
				in >> ini.port;
			}
			else if(buffer == "clients")
			{
				in >> ini.clients;
			}

		}

		in.close();
		return true;
	}
}//End namespace DanBias
