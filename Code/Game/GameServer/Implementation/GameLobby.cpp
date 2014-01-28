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
	{  }

	void GameLobby::Frame()
	{
		ParseEvents();
	}

	void GameLobby::ClientEventCallback(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e)
	{

	}
	void GameLobby::ClientConnectedEvent(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e)
	{

	}

//////// Private
	void GameLobby::ParseEvents()
	{
		//if(this->box && !this->box->IsEmpty())
		//{
		//	NetEvent &e = this->box->Fetch();
		//
		//	short type = e.protocol[0].value.netShort;
		//
		//		 if(ProtocolIsLobby(type))		ParseLobbyProtocol(e.protocol, e.sender);
		//	else if(ProtocolIsGeneral(type))	ParseGeneralProtocol(e.protocol, e.sender);
		//}
	}

}//End namespace DanBias