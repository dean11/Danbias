/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>
#include <WindowShell.h>
#include <iostream>


#include "..\GameServer.h"
#include "..\GameSessionManager.h"
#include "..\GameSession.h"

#include <Thread\OysterThread.h>
#include <Utilities.h>
#include <NetworkClient.h>
#include <thread>
#include <CollisionManager.h>

namespace DanBias
{
	using namespace Oyster::Network;

	GameServer* instance = 0;
	std::thread workerThread;
	typedef void(*WorkerThreadFnc)(GameServer*);

	DanBiasServerReturn GameServerAPI::Create(const GameInitDesc& desc)
	{
		if(!instance)
			instance = new GameServer();

		return instance->Create(desc);
	}

	

	GameServer::GameServer()
		:	maxClients(0)
	{  }
	GameServer::~GameServer()
	{  }
	DanBiasServerReturn GameServer::Create(const GameServerAPI::GameInitDesc& desc)
	{
		this->maxClients = desc.maxNumberOfClients;
		this->server = new NetworkServer();
		this->lobby = new GameLobby();

		if(desc.threaded)
		{
			if(!this->server->Init(desc.connectionPort, this->lobby))
				return DanBiasServerReturn_Error;

			if(!this->server->Start())	
				return DanBiasServerReturn_Error;

			WorkerThreadFnc temp = GameServer::Run;
			workerThread = std::thread(temp, this);
		}
		else
		{
			if(!this->server->Init(desc.connectionPort, this->lobby))
				return DanBiasServerReturn_Error;

			if(!this->server->Start())	
				return DanBiasServerReturn_Error;

			Run();
		}

		return DanBiasServerReturn_Sucess;
	}
	void GameServer::Run(GameServer* owner)
	{
		while (true)
		{
			owner->server->ProcessConnectedClients();
			owner->lobby->ProcessClients();

			if(GetAsyncKeyState(0x51))	//Q for exit
				break;
		}
	}
	void GameServer::Run()
	{
		while (true)
		{
			this->server->ProcessConnectedClients();
			this->lobby->Frame();

			if(GetAsyncKeyState(0x51))	//Q for exit
				break;
		}
	}
	void GameServer::Release()
	{
		GameSessionManager::CloseSessions();
	}




	//void GameServer::ClientConnected(NetworkClient* client)
	//{
	//	static bool myTest = false;
	//	static int sessionId = -1;
	//	printf("Client with ID [%i] connected.\n", client->GetID());
	//
	//	if(!myTest)
	//	{
	//		Utility::DynamicMemory::SmartPointer<NetworkSession> c = new Client(client);
	//
	//		GameSessionDescription desc;
	//		desc.mapName = L"test";
	//		desc.clients.Push(c);
	//		desc.exitDestionation = this->lobby;
	//		if((sessionId = GameSessionManager::AddSession(desc, true)) == 0)
	//			printf("Failed to create a game session\n");
	//		myTest = true;
	//		//myTest = new GameSession();
	//		//
	//		//DanBias::GameSession::GameSessionDescription desc;
	//		//desc.owner = 0;
	//		//desc.clients.Push(c);
	//		//
	//		//if(!myTest->Create(desc)) return;
	//		//myTest->Run();
	//	}
	//	else
	//	{
	//		Utility::DynamicMemory::SmartPointer<NetworkSession> c = new NetworkSession(client);
	//		GameSessionManager::JoinSession(sessionId, c);
	//	}
	//	
	//
	//	//Utility::DynamicMemory::SmartPointer<NetworkSession> c = new NetworkSession(client);
	//	//this->mainLobby->Attach(c, this->mainLobby->GetPostbox());
	//}
}//End namespace DanBias
