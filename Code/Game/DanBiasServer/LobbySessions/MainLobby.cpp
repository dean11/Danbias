/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "MainLobby.h"
#include "LobbyClient.h"
#include <PlayerProtocols.h>
#include <PostBox\PostBox.h>

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;
using namespace Oyster;

namespace DanBias
{
	MainLobby::MainLobby()
		:gameLobby(5)
	{
		this->box = new PostBox<DanBias::NetworkSession::NetEvent>();
	}
	MainLobby::~MainLobby()
	{
		
	}
	void MainLobby::Release()
	{
		delete this->box;
		this->box = 0;
		this->CloseSession(true);
	}

	void MainLobby::Frame()
	{
		ParseEvents();
	}
	IPostBox<NetworkSession::NetEvent>* MainLobby::GetPostbox()
	{
		return this->box;
	}

//////// Private
	void MainLobby::ParseEvents()
	{
		if(this->box && !this->box->IsEmpty())
		{
			NetEvent &e = this->box->Fetch();

			ParseProtocol(e.protocol, e.sender);
			
		}
	}
	void MainLobby::ParseProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::LobbyClient* c)
	{
		bool update = false;
		switch (p[0].value.netShort)
		{
			case protocol_Lobby_CreateGame:
			{
				GameLogic::Protocol_LobbyCreateGame val(p);
				CreateGame(val, c);
				update = true;
			}
			break;
			case protocol_Lobby_JoinLobby:
			{
				GameLogic::Protocol_LobbyJoinLobby val(p);
				JoinLobby(val, c);
			}
			break;
			case protocol_Lobby_LeaveLobby:
			{
				Detach(c)->Disconnect();
			}
			break;
		}

		if(update)	SendUpdate();
	}

	void MainLobby::CreateGame(GameLogic::Protocol_LobbyCreateGame& p, DanBias::LobbyClient* c)
	{
		for (unsigned int i = 0; i < this->gameLobby.Size(); i++)
		{
			if(!gameLobby[i])
			{
				gameLobby[i] = new GameLobby(NetworkSession::Detach(c));
				return;
			}
		}

		this->gameLobby.Push(new GameLobby(NetworkSession::Detach(c)));
	}
	void MainLobby::JoinLobby(GameLogic::Protocol_LobbyJoinLobby& p, DanBias::LobbyClient* c)
	{
		for (unsigned int i = 0; i < this->gameLobby.Size(); i++)
		{
			if (this->gameLobby[i]->GetID() == p.LobbyID)
			{
				this->gameLobby[i]->Attach(Detach(c));
				return;
			}
		}
	}

	void MainLobby::SendUpdate()
	{
		//Send Lobbys
		GameLogic::Protocol_LobbyRefresh();
	}

}//End namespace DanBias