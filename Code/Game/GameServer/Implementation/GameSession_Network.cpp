/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\GameSession.h"
#include "..\GameClient.h"

#include <Protocols.h>
#include <PostBox\PostBox.h>
#include <GameLogicStates.h>
#include <Windows.h>

using namespace Utility::DynamicMemory;
using namespace Oyster;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace GameLogic;

namespace DanBias
{
	void GameSession::Send(Oyster::Network::CustomNetProtocol* p)
	{
		for (unsigned int i = 0; i < this->clients.Size(); i++)
		{
			if(this->clients[i] && this->clients[i]->GetClient())
				this->clients[i]->GetClient()->Send(p);
		}
	}
}//End namespace DanBias

