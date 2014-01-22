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
	void MainLobby::SetRefreshFrequency(float delta)
	{
		this->refreshFrequency = delta;
	}

	float MainLobby::GetRefreshFrequency() const
	{
		return this->refreshFrequency;
	}
//////// Private
	void MainLobby::ParseEvents()
	{
		if(this->box && !this->box->IsEmpty())
		{
			NetEvent &e = this->box->Fetch();

			short type = e.protocol[0].value.netShort;

				 if(ProtocolIsLobby(type))		ParseLobbyProtocol(e.protocol, e.sender);
			else if(ProtocolIsGeneral(type))	ParseGeneralProtocol(e.protocol, e.sender);
		}
	}

}//End namespace DanBias