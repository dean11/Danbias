/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>
#include <WindowShell.h>
#include <iostream>

#include "GameServer.h"
#include "Helpers\ServerDataReader.h"
#include "GameSession\GameSessionManager.h"
#include "LobbySessions\LobbyClient.h"
#include "GameSession\GameSession.h"
#include "AdminInterface\AdminInterface.h"

#include <Thread\OysterThread.h>
#include <Utilities.h>

#include <CollisionManager.h>

namespace DanBias
{
	using namespace Oyster::Network;

	GameServer* GameServer::instance = 0;

	void GameServer::NetworkCallback(NetworkClient* client)
	{
		static bool myTest = false;
		static int sessionId = -1;
		printf("Client with ID [%i] connected.\n", client->GetID());

		if(!myTest)
		{
			Utility::DynamicMemory::SmartPointer<LobbyClient> c = new LobbyClient(client);

			GameSessionDescription desc;
			desc.mapName = L"test";
			desc.clients.Push(c);
			desc.exitDestionation = this->mainLobby;
			if((sessionId = GameSessionManager::AddSession(desc, true)) == 0)
				printf("Failed to create a game session\n");
			myTest = true;
			//myTest = new GameSession();
			//
			//DanBias::GameSession::GameSessionDescription desc;
			//desc.owner = 0;
			//desc.clients.Push(c);
			//
			//if(!myTest->Create(desc)) return;
			//myTest->Run();
		}
		else
		{
			Utility::DynamicMemory::SmartPointer<LobbyClient> c = new LobbyClient(client);
			GameSessionManager::JoinSession(sessionId, c);
		}
		

		//Utility::DynamicMemory::SmartPointer<LobbyClient> c = new LobbyClient(client);
		//this->mainLobby->Attach(c, this->mainLobby->GetPostbox());
	}


	GameServer::GameServer()
		:	initiated(0)
		,	running(0)
		,	released(0)
		,	maxClients(0)
		,	mainLobby(0)
		,	server(0)
	{ this->instance = this; }
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

		this->initiated = true;
		return DanBiasServerReturn_Sucess;
	}
	DanBiasServerReturn GameServer::Run()
	{
		if(this->running)			return DanBiasServerReturn_Error;
		if(this->released)			return DanBiasServerReturn_Error;
		if(!this->initiated)		return DanBiasServerReturn_Error;

		if(!this->server->Start())	return DanBiasServerReturn_Error;
		//Oyster::Thread::OysterThread ioThread;
		//ioThread.Create(this, true, 

		while (true)
		{
			if(!WindowShell::Frame())	break;

			this->mainLobby->Frame();

			if(GetAsyncKeyState(0x51))	//Q for exit
				break;
		}

		return DanBiasServerReturn_Sucess;
	}
	DanBiasServerReturn GameServer::Release()
	{
		GameSessionManager::CloseSession();
		this->mainLobby->Release();
		delete this->mainLobby;
		this->server->Shutdown();
		delete this->server;
		this->released = true;
		return DanBiasServerReturn_Sucess;
	}

	NetworkSession* GameServer::MainLobbyInstance()
	{
		return GameServer::instance->mainLobby;
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
