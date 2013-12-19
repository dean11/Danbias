/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>
#include <WindowShell.h>
#include <iostream>

#include "GameServer.h"
#include "Utilities.h"
#include "Helpers\ServerInitReader.h"
#include <Thread\OysterThread.h>
#include "ServerObjects\ClientObject.h"
#include "ServerObjects\GameSession.h"

#include <CollisionManager.h>

namespace DanBias
{
	using namespace Oyster::Network;

	static GameSession *myTest;

	void GameServer::ClientConnectCallback(NetworkClient* client)
	{
		if(!myTest)
		{
			myTest = new GameSession();
			Utility::DynamicMemory::SmartPointer<ClientObject> c = new ClientObject(client);
			DanBias::GameSession::GameSessionDescription desc;
			desc.owner = 0;
			desc.clients.Push(c);

			myTest->Run(desc);
		}

		//printf("Client connected with socket: %i\n", client->Id());
		//
		//Utility::DynamicMemory::SmartPointer<ClientObject> c = new ClientObject(client);
		//this->mainLobby->AttachClient(c, this->mainLobby->GetPostbox());
	}
	GameServer::GameServer()
		:	initiated(0)
		,	running(0)
		,	released(0)
		,	maxClients(0)
		,	mainLobby(0)
		,	server(0)
	{ }
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
		if(!WindowShell::CreateWin(WindowShell::WINDOW_INIT_DESC()))							return DanBiasServerReturn_Error;

		this->initiated = true;
		return DanBiasServerReturn_Sucess;
	}
	DanBiasServerReturn GameServer::Run()
	{
		if(this->running)			return DanBiasServerReturn_Error;
		if(this->released)			return DanBiasServerReturn_Error;
		if(!this->initiated)		return DanBiasServerReturn_Error;

		if(!this->server->Start())	return DanBiasServerReturn_Error;

		while (true)
		{
			if(!WindowShell::Frame())	break;

			this->mainLobby->Frame();
		}

		return DanBiasServerReturn_Sucess;
	}
	DanBiasServerReturn GameServer::Release()
	{
		delete this->mainLobby;
		this->server->Shutdown();
		delete this->server;
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
