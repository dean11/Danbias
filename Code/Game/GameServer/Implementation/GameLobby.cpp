/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\GameLobby.h"
#include <PlayerProtocols.h>
#include <PostBox\PostBox.h>
#include <Protocols.h>

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;
using namespace Oyster;
using namespace GameLogic;

using namespace DanBias;

GameLobby::GameLobby()
{   }
GameLobby::~GameLobby()
{  
	this->clients.Clear();
}
void GameLobby::Release()
{  
	NetworkSession::CloseSession(true);
	this->gameSession.CloseSession(true);
}
void GameLobby::Update()
{
	for (unsigned int i = 0; i < this->clients.Size(); i++)
	{
		if(this->clients[i])
		{
			this->clients[i]->Update();
		}
	}
}
void GameLobby::SetGameDesc(const LobbyLevelData& desc)
{
	this->description.gameMode = desc.gameMode;
	this->description.gameTime = desc.gameTime;
	this->description.mapNumber = desc.mapNumber;
	this->description.maxClients = desc.maxClients;
}
void GameLobby::GetGameDesc(LobbyLevelData& desc)
{
	desc.gameMode = this->description.gameMode;
	desc.gameTime = this->description.gameTime;		
	desc.mapNumber = this->description.mapNumber;		
	desc.maxClients = this->description.maxClients;	
}
bool GameLobby::StartGameSession(  )
{
//Check if all clients is ready
	if(this->GetClientCount() && this->GetClientCount() == this->readyList.Size())
	{
		GameSession::GameDescription desc;
			desc.maxClients = this->description.maxClients;
			desc.gameMode = this->description.gameMode;
			desc.gameTimeMinutes = this->description.gameTime;
			//desc.mapName = this->description.mapNumber;
			desc.owner = this;
			desc.clients = this->clients;

		if(desc.gameTimeMinutes == 0)
			desc.gameTimeMinutes = 10; //note: should be fetched from somewhere.

		if(desc.maxClients == 0)
			desc.maxClients = 10; //note: should be fetched somewhere else..

		this->clients.Clear();	//Remove clients from lobby list
		
		if(this->gameSession.Create(desc))
		{
			this->gameSession.Run();

			return true;
		}
	}
	else
	{
		//?
	}
	return false;
}

void GameLobby::ClientEventCallback(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e)
{
	switch (e.args.type)
	{
		case NetworkClient::ClientEventArgs::EventType_Disconnect:

		break;
		case NetworkClient::ClientEventArgs::EventType_ProtocolFailedToRecieve:

		break;
		case NetworkClient::ClientEventArgs::EventType_ProtocolFailedToSend:
			printf("\t(%i : %s) - EventType_ProtocolFailedToSend\n", e.sender->GetID(), e.sender->GetIpAddress().c_str());	
			e.sender->Disconnect();
			this->readyList.Remove(e.sender);
			this->clients.Remove(e.sender);
		break;
		case NetworkClient::ClientEventArgs::EventType_ProtocolRecieved:
			printf("\t(%i : %s) - EventType_ProtocolRecieved\n", e.sender->GetID(), e.sender->GetIpAddress().c_str());	
			this->ParseProtocol(e.args.data.protocol, e.sender);
		break;
	}
}
void GameLobby::ClientConnectedEvent(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client)
{
	printf("New client(%i) connected - %s \n", client->GetID(), client->GetIpAddress().c_str());

	if(this->gameSession)
	{
		Attach(client);
	}
	else
	{
		Attach(client);
		Protocol_LobbyClientData p1;
		Protocol_LobbyGameData p2;
		
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i])
			{
				Protocol_LobbyClientData::PlayerData t;
				t.id = this->clients[i]->GetID();
				t.ip = this->clients[i]->GetIpAddress();
				t.team = 0;
				t.name = "Dennis �r kung tycker Erik!";
				p1.list.Push(t);
			}
		}
		p2.majorVersion = 1;
		p2.minorVersion = 0;
		p2.mapName = "Dennis �r kung tycker Erik!";

		client->Send(p1.GetProtocol());
		client->Send(p2.GetProtocol());
	}
}

