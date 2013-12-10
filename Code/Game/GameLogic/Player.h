//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef PLAYER_H
#define PLAYER_H
#include "GameLogicStates.h"
#include "OysterMath.h"

namespace GameLogic
{
	class Player
	{

	public:
		Player(void);
		~Player(void);

		void Update();
		void Move(const PLAYER_MOVEMENT &movement);
		void Shoot(const WEAPON_FIRE &fireInput);
		void Jump();

		bool IsWalking();
		bool IsJumping();
		bool IsIdle();

		Oyster::Math::Float3 GetPos();
		void Respawn();

	private:
		struct PrivateData;
		PrivateData *myData;
	};
}
#endif