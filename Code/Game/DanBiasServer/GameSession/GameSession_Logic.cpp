/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "GameSession.h"
#include "GameClient.h"

#include <Protocols.h>
#include <PostBox\PostBox.h>
#include <GameLogicStates.h>
#include <Windows.h>
#include <OysterMath.h>

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
		if(GetAsyncKeyState(VK_DOWN))
		{
			Oyster::Math::Float4x4 world = Oyster::Math::Matrix::identity;
			Protocol_CreateObject p(world, 2, "../Content/crate");
			Send(p.GetProtocol());
			Sleep(100);
		}

		return this->isRunning;
	}

}//End namespace DanBias

