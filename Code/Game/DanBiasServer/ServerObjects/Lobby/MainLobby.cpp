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
		if(!this->box.IsEmpty())
		{
			NetEvent &e = this->box.Fetch();
			ParseEvent(e);
		}
	}

//////// Private
	void MainLobby::ParseEvent(NetEvent& e)
	{
		static const short i = MAXSHORT;
		if(e.protocol[0].type != Oyster::Network::NetAttributeType_Short) return;

		short f = e.protocol[0].value.netShort;

		switch (f)
		{
		default:
			break;
		}
	}

}//End namespace DanBias