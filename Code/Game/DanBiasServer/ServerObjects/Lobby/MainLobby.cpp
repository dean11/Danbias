#include "MainLobby.h"
#include <PlayerProtocols.h>
#include <PostBox\PostBox.h>

namespace DanBias
{
	MainLobby::MainLobby()
	{
		this->box = new Oyster::PostBox<DanBias::NetworkSession::NetEvent>();
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

//////// Private
	void MainLobby::ParseEvents()
	{
		if(this->box && !this->box->IsEmpty())
		{
			NetEvent &e = this->box->Fetch();

			if(e.protocol[0].type != Oyster::Network::NetAttributeType_Short) return;

			short f = e.protocol[0].value.netShort;

			switch (f)
			{
				default:
					
				break;
			}
		}
	}

}//End namespace DanBias