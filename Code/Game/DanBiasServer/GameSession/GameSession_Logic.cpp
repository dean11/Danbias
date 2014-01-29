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
	//TEST SHIT
	GameAPI *game = &GameAPI::Instance();
	DynamicArray<IPlayerData*> players;

	//TEST SHIT

	bool GameSession::DoWork(  )
	{
		if(this->isRunning)
		{
			//TEST SHIT
			//player creation and testing
			//players.Resize(10);

			//for(int i = 0; i < 10; i++)
			//{
			//	players[i] = game->CreatePlayer();WWW
			//	players[i]->Move(GameLogic::PLAYER_MOVEMENT::PLAYER_MOVEMENT_BACKWARD);
			//	players[i]->Move(GameLogic::PLAYER_MOVEMENT::PLAYER_MOVEMENT_FORWARD);
			//	players[i]->Move(GameLogic::PLAYER_MOVEMENT::PLAYER_MOVEMENT_JUMP);
			//	players[i]->Move(GameLogic::PLAYER_MOVEMENT::PLAYER_MOVEMENT_LEFT);
			//	players[i]->Move(GameLogic::PLAYER_MOVEMENT::PLAYER_MOVEMENT_RIGHT);

			//	//using weapon testing
			//	players[i]->UseWeapon(GameLogic::WEAPON_FIRE::WEAPON_USE_PRIMARY_PRESS);
			//	players[i]->UseWeapon(GameLogic::WEAPON_FIRE::WEAPON_USE_PRIMARY_RELEASE);
			//	players[i]->UseWeapon(GameLogic::WEAPON_FIRE::WEAPON_USE_SECONDARY_PRESS);
			//	players[i]->UseWeapon(GameLogic::WEAPON_FIRE::WEAPON_USE_SECONDARY_RELEASE);
			//	players[i]->UseWeapon(GameLogic::WEAPON_FIRE::WEAPON_USE_UTILLITY_PRESS);
			//}
			

			//TEST SHIT
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
			Protocol_ObjectPosition p(world, 2);
			Send(p.GetProtocol());
		}
	}

}//End namespace DanBias

