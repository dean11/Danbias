/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\GameLobby.h"
#include <PlayerProtocols.h>
#include <PostBox\PostBox.h>

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
			break;
			case NetworkClient::ClientEventArgs::EventType_ProtocolRecieved:
				this->ParseProtocol(e.args.data.protocol, e.sender);
			break;
		}
	}
	void GameLobby::ClientConnectedEvent(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client)
	{
		//Attach(client);
	}

}//End namespace DanBias