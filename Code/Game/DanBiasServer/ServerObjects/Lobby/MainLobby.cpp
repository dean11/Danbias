#include "MainLobby.h"


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
		this->clients.clear();
	}

	void MainLobby::Frame()
	{
		if(!this->box.IsEmpty())
		{

		}
	}
}//End namespace DanBias