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

namespace DanBias
{
	GameLobby::GameLobby()
	{   }

	GameLobby::~GameLobby()
	{  }

	void GameLobby::Release()
	{  
		NetworkSession::CloseSession(true);
	}

	void GameLobby::Update()
	{
		if(GetAsyncKeyState(VK_DOWN))
			this->Send(*GameLogic::Protocol_General_Status().GetProtocol());

		this->ProcessClients();
	}
	GameLobby::operator bool()
	{
		return true;
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
		Attach(client);
	}

}//End namespace DanBias