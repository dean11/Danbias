/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "GameSession.h"
#include "GameClient.h"

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
	bool GameSession::DoWork(  )
	{
		this->gameInstance.NewFrame();

		this->ParseEvents();

		if(GetAsyncKeyState(VK_UP))
		{
			Protocol_General_Status p(Protocol_General_Status::States_ready);
			Send(p.GetProtocol());
			Sleep(100);
		}

		return this->isRunning;
	}

}//End namespace DanBias

