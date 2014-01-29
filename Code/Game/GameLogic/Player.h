//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef PLAYER_H
#define PLAYER_H

#include "GameLogicStates.h"
#include "OysterMath.h"
#include "DynamicObject.h"


namespace GameLogic
{
	class Weapon;
	class Player : public DynamicObject
	{
	public:
		Player(void);
		Player(OBJECT_TYPE type);
		Player(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type);
		Player(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type);
		Player(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type);
		Player(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type);
		~Player(void);
		void InitPlayer();
		
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
		void UseWeapon(const WEAPON_FIRE &usage);

		/********************************************************
		* Respawns the player, this resets several stats and settings on the player
		* @param spawnPoint: the coordinate in the world where the player is to spawn
		********************************************************/
		void Respawn(Oyster::Math::Float3 spawnPoint);


		void Rotate(const Oyster::Math3D::Float3 lookDir);

		/********************************************************
		* Collision function for player, this is to be sent to physics through the subscribe function with the rigidbody
		* Will be called when the physics detect a collision
		* @param rigidBodyPlayer: physics object of the player
		* @param obj: physics object for the object that collided with the player
		********************************************************/
		static void PlayerCollision(Oyster::Physics::ICustomBody *rigidBodyPlayer, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);

		
		bool IsWalking();
		bool IsJumping();
		bool IsIdle();

		Oyster::Math::Float3 GetPosition() const;
		Oyster::Math::Float3 GetLookDir() const;
		Oyster::Math::Float4x4 GetOrientation() const;
		int GetTeamID() const;
		PLAYER_STATE GetState() const;

		void DamageLife(int damage);

	private:
		void Jump();

	private:
		int life;
		int teamID;
		Weapon *weapon;
		PLAYER_STATE playerState;
		Oyster::Math::Float3 lookDir;

		bool hasTakenDamage;
		float invincibleCooldown;

	};
}
#endif