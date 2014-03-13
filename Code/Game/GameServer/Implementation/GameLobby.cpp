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
{  }
GameLobby::~GameLobby()
{  
	this->gClients.Clear();
}
void GameLobby::Release()
{  
	NetworkSession::CloseSession(true);
	this->gameSession.CloseSession(true);
}
void GameLobby::Update()
{
	this->ProcessClients();
}
void GameLobby::SetGameDesc(const LobbyLevelData& desc)
{
	this->description.gameMode = desc.gameMode;
	this->description.gameName = desc.gameName;
	this->description.mapName = desc.mapName;
	this->description.gameTimeInMinutes = desc.gameTimeInMinutes;
	this->description.maxClients = desc.maxClients;
	
	if(this->gClients.Size() > (unsigned int)desc.maxClients)
	{
		//Kick overflow
		for (unsigned int i = (unsigned int)desc.maxClients - 1; i < this->gClients.Size(); i++)
		{
			if(this->gClients[i])
			{
				this->gClients[i]->GetClient()->Disconnect();
			}
		}
	}
	this->gClients.Resize((unsigned int)desc.maxClients);

}
void GameLobby::GetGameDesc(LobbyLevelData& desc)
{
	desc.gameTimeInMinutes = this->description.gameTimeInMinutes;		
	desc.maxClients = this->description.maxClients;	
	desc.mapName = this->description.mapName;		
	desc.gameName = this->description.gameName;	
	desc.gameMode = this->description.gameMode;
	
}
bool GameLobby::StartGameSession( bool forceStart )
{
//Check if all clients is ready, in not force start
	if(!forceStart)
	{
		if(!this->GetClientCount())
		{ /*None connected*/ return false;}
		else if( this->GetClientCount() != this->readyList.Size() )
		{ /*Not enough connected*/ return false; }
	}

	GameSession::GameDescription desc;
		desc.maxClients = this->description.maxClients;
		desc.gameMode = this->description.gameMode;
		desc.gameTimeMinutes = this->description.gameTimeInMinutes;
		desc.mapName = this->description.mapName;
		desc.owner = this;
		desc.clients = this->gClients;

	if(desc.gameTimeMinutes == 0)
		desc.gameTimeMinutes = 10; //note: should be fetched from somewhere.

	if(desc.maxClients == 0)
		desc.maxClients = 10; //note: should be fetched somewhere else..

	this->gClients.Clear();	//Remove clients from lobby list
		
	if(this->gameSession.Create(desc, forceStart))
	{
		this->gameSession.Run();

		return true;
	}
	
	
	return false;
}
int GameLobby::GetGameSessionClientCount()
{
	return this->gameSession.GetClientCount();
}
float GameLobby::GetGameTimeLeft() const
{
	return this->gameSession.GetElapsedSeconds();
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
			//e.sender->Disconnect();
			//this->readyList.Remove(e.sender);
			//this->gClients.Remove(e.sender);
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
		if(!this->Attach(client))
		{
			client->Disconnect();
		}
	}
	else
	{
		if(!this->Attach(client))
		{
			//Send message that lobby full
			client->Disconnect();
			return;
		}

		Protocol_LobbyClientData p1;
		Protocol_LobbyGameData p2;
		
		for (unsigned int i = 0; i < this->gClients.Size(); i++)
		{
			if(this->gClients[i])
			{
				Protocol_LobbyClientData::PlayerData t;
				t.id = client->GetID();
				t.ip = client->GetIpAddress();
				t.team = 0;
				t.name = "Dennis är kung tycker Erik!";
				p1.list.Push(t);
			}
		}
		p2.majorVersion = 1;
		p2.minorVersion = 0;
		p2.mapName = "Dennis är kung tycker Erik!";

		client->Send(p1.GetProtocol());
		client->Send(p2.GetProtocol());
	}
}
void GameLobby::ProcessClients()
{
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(this->gClients[i])
		{
			this->gClients[i]->UpdateClient();
		}
	}
}
bool GameLobby::Attach(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client) 
{
	if(this->clientCount == this->description.maxClients) return false;

	client->SetOwner(this);

	bool added = false;
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(!this->gClients[i])
		{
			added = true;
			this->gClients[i] = new GameClient(client);
		}
	}

	if(!added)
	{
		this->gClients.Push(new GameClient(client));
	}
	return true;
}
bool GameLobby::Attach(Utility::DynamicMemory::SmartPointer<GameClient> gClient)
{
	if(this->clientCount == this->description.maxClients) return false;

	gClient->SetOwner(this);

	bool added = false;
	for (unsigned int i = 0; i < this->gClients.Size(); i++)
	{
		if(!this->gClients[i])
		{
			added = true;
			this->gClients[i] = gClient;
		}
	}

	if(!added)
	{
		this->gClients.Push(gClient);
	}
	return true;
}








