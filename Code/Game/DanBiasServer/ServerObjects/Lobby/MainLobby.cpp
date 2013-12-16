#include "MainLobby.h"
#include <PlayerProtocols.h>

namespace DanBias
{
	MainLobby::MainLobby()
	{

	}
	MainLobby::~MainLobby()
	{

	}
	void MainLobby::Release()
	{
		this->DetachClient();
	}

	void MainLobby::Frame()
	{
		ParseEvents();
	}

//////// Private
	void MainLobby::ParseEvents()
	{
		if(!this->box.IsEmpty())
		{
			NetEvent &e = this->box.Fetch();

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