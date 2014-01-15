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

		this->ParseEvents();

		this->gameInstance.NewFrame();

		this->UpdateGameObjects();

		return this->isRunning;
	}

	void GameSession::UpdateGameObjects()
	{
		while(!this->modifiedClient.IsEmpty())
		{
			//There is something that needs update
			GameSessionEvent e = this->modifiedClient.Pop();
			
			switch (e.value)
			{
				case GameSessionEvent::EventType_Player:
					//e.data.player->GetOrientation();
				break;
			}
		}
	}

}//End namespace DanBias

