//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef PLAYER_H
#define PLAYER_H
#include "GameLogicStates.h"
#include "OysterMath.h"
#include "Object.h"
#include "GameLogicDef.h"

namespace GameLogic
{
	class DANBIAS_GAMELOGIC_DLL Player : public Object
	{

	public:
		Player(void);
		~Player(void);

		void Update();
		void Move(const PLAYER_MOVEMENT &movement);
		void UseWeapon(const WEAPON_FIRE &fireInput);
		void Jump();

		bool IsWalking();
		bool IsJumping();
		bool IsIdle();

		Oyster::Math::Float3 GetPos();
		Oyster::Math::Float3 GetLookDir();

		void Respawn();

		void DamageLife(int damage);

	private:
		struct PrivateData;
		PrivateData *myData;
	};
}
#endif