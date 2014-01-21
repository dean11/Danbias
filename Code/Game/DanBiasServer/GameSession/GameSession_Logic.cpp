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

#define DELTA_TIME_20	0.05f
#define DELTA_TIME_24	0.04166666666666666666666666666667f
#define DELTA_TIME_30	0.03333333333333333333333333333333f
#define DELTA_TIME_60	0.01666666666666666666666666666667f
#define DELTA_TIME_120	0.00833333333333333333333333333333f

using namespace Utility::DynamicMemory;
using namespace Oyster;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace GameLogic;

namespace DanBias
{
	bool GameSession::DoWork(  )
	{
		if(this->isRunning)
		{
			if(GetAsyncKeyState(VK_UP))
			{
				Protocol_General_Status p(Protocol_General_Status::States_ready);
				Send(p.GetProtocol());
				Sleep(100);
			}
			if(GetAsyncKeyState(VK_DOWN))
			{
				Oyster::Math::Float4x4 world = Oyster::Math::Matrix::identity;
				Protocol_ObjectCreate p(world, 2, "../Content/crate");
				Send(p.GetProtocol());
				Sleep(100);
			}
		
			double dt = this->timer.getElapsedSeconds();
			gameInstance.SetFrameTimeLength((float)dt);

			if(dt >= DELTA_TIME_20)
			{
				this->ParseEvents();

				this->gameInstance.NewFrame();

				this->UpdateGameObjects();

				this->timer.reset();
			}
		}

		return this->isRunning;
	}

	void GameSession::UpdateGameObjects()
	{
		if(clients.Size() >= 1 && clients[0])
		{
			Oyster::Math::Float4x4 world = 	this->clients[0]->GetPlayer()->GetOrientation();
			Protocol_ObjectPosition p(world, 1);
			Send(p.GetProtocol());
		}
	}

}//End namespace DanBias

