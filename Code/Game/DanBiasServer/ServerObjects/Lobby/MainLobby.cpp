
#include "MainLobby.h"
#include "..\..\Helpers\ProtocolParser.h"
#include "..\ClientObject.h"
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
		delete this->box;
		this->box = 0;
	}
	void MainLobby::Release()
	{
		this->CloseSession(0);
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

			ParseProtocol(e.protocol, *e.reciever);
			
		}
	}
	void MainLobby::ParseProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::ClientObject& c)
	{
		//switch (p[0].value.netChar)
		//{
		//	case protocol_Lobby_CreateGame:
		//	{
		//		GameLogic::Protocol_LobbyCreateGame val(p);
		//		CreateGame(val, c);
		//	}
		//	break;
		//	case protocol_Lobby_JoinGame:
		//	{
		//
		//	}
		//	break;
		//	case protocol_Lobby_JoinLobby:
		//	{
		//
		//	}
		//	break;
		//	case protocol_Lobby_LeaveLobby:
		//	{
		//
		//	}
		//	break;
		//}

	}

	void MainLobby::CreateGame(GameLogic::Protocol_LobbyCreateGame& p, DanBias::ClientObject& c)
	{
		SmartPointer<ClientObject> sc = NetworkSession::FindClient(c);
		NetworkSession::DetachClient(&c);

		if(!sc) return;

		for (unsigned int i = 0; i < this->gameLobby.Size(); i++)
		{
			if(!gameLobby[i])
			{
				gameLobby[i] = new GameLobby(sc);
				return;
			}
		}

		this->gameLobby.Push(new GameLobby(sc));
	}

}//End namespace DanBias