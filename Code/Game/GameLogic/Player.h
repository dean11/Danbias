//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef PLAYER_H
#define PLAYER_H

#include "GameLogicStates.h"
#include "OysterMath.h"
#include "Object.h"
#include "OysterMath.h"
#include "CollisionManager.h"



namespace GameLogic
{
	class Weapon;
	class Player : public Object
	{

	public:
		Player(void);
		~Player(void);
		
		/********************************************************
		* Moves the player based on input
		* @param movement: enum value on what kind of action is to be taken
		********************************************************/
		void Move(const PLAYER_MOVEMENT &movement);

		void MoveForward();
		void MoveBackwards();
		void MoveRight();
		void MoveLeft();

		/********************************************************
		* Uses the weapon based on input
		* @param fireInput: enum value on what kind of action is to be taken
		********************************************************/
		void UseWeapon(const WEAPON_FIRE &Usage);

		/********************************************************
		* Respawns the player, this resets several stats and settings on the player
		* @param spawnPoint: the coordinate in the world where the player is to spawn
		********************************************************/
		void Respawn(Oyster::Math::Float3 spawnPoint);

		
		bool IsWalking();
		bool IsJumping();
		bool IsIdle();

		Oyster::Math::Float3 GetPosition() const;
		Oyster::Math::Float3 GetLookDir() const;
		Oyster::Math::Float4x4 GetOrientation() const;
		int GetTeamID() const;
		PLAYER_STATE GetState() const;

		void DamageLife(int damage);

		//Do frame calculations
		void BeginFrame();
		void EndFrame();

	private:
		void Jump();

	private:
		int life;
		int teamID;
		Weapon *weapon;
		PLAYER_STATE playerState;
		Oyster::Math::Float4 lookDir;

	};
}
#endif