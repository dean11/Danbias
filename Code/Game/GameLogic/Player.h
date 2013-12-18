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
		
		/********************************************************
		* Moves the player based on input
		* @param movement: enum value on what kind of action is to be taken
		********************************************************/
		void Move(const PLAYER_MOVEMENT &movement);

		/********************************************************
		* Uses the weapon based on input
		* @param fireInput: enum value on what kind of action is to be taken
		********************************************************/
		void UseWeapon(const WEAPON_FIRE &fireInput);

		/********************************************************
		* Respawns the player, this resets several stats and settings on the player
		* @param spawnPoint: the coordinate in the world where the player is to spawn
		********************************************************/
		void Respawn(Oyster::Math::Float3 spawnPoint);

		
		bool IsWalking();
		bool IsJumping();
		bool IsIdle();

		Oyster::Math::Float3 GetPos();
		Oyster::Math::Float3 GetLookDir();
		int GetTeamID();

		void DamageLife(int damage);

	private:

		void Jump();
	private:
		struct PrivateData;
		PrivateData *myData;
	};
}
#endif