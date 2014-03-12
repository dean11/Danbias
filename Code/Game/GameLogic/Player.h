//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef PLAYER_H
#define PLAYER_H

#include "GameLogicStates.h"
#include "OysterMath.h"
#include "DynamicObject.h"
#include "DynamicArray.h"

const float MAX_HP = 100.0f;
const float BASIC_SPEED = 10.0f;

namespace GameLogic
{
	class Weapon;
	class Player : public DynamicObject
	{
	public:
		struct PlayerStats
		{
			Oyster::Math::Float hp;
			Oyster::Math::Float movementSpeed;
			//Oyster::Math::Float resistance;
		};

		struct PlayerScore
		{
			int killScore;
			int deathScore;
			// int assistScore;
			// int suicideScore;
		};

		Player(void);

		Player(Oyster::Physics::ICustomBody *rigidBody, void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID);
		Player(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID);

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
		void UseWeapon(const WEAPON_FIRE &usage);
		void SelectWeaponAttatchment( int socketID );
		/********************************************************
		* Respawns the player, this resets several stats and settings on the player
		* @param spawnPoint: the coordinate in the world where the player is to spawn
		********************************************************/
		void Respawn(Oyster::Math::Float3 spawnPoint);


		void SetLookDir(const Oyster::Math3D::Float3& lookDir);

		void TurnLeft(Oyster::Math3D::Float deltaRadians);

		/********************************************************
		* Collision function for player, this is to be sent to physics through the subscribe function with the rigidbody
		* Will be called when the physics detect a collision
		* @param rigidBodyPlayer: physics object of the player
		* @param obj: physics object for the object that collided with the player
		********************************************************/
		static void PlayerCollision(Oyster::Physics::ICustomBody *objA, Oyster::Physics::ICustomBody *objB, Oyster::Math::Float kineticEnergyLoss);

		
		bool IsWalking();
		bool IsJumping();
		bool IsIdle();
		bool IsStunned( bool struggled = false );

		void ResetPlayer( Oyster::Math::Float3 spawnPos);

		Oyster::Math::Float3 GetPosition() const;
		Oyster::Math::Float3 GetLookDir() const;
		Oyster::Math::Float4x4 GetOrientation() const;
		int GetTeamID() const;
		PLAYER_STATE GetState() const;
		Oyster::Math::Float GetRecentlyAffected();

		void AddKill();
		void AddDeath();
		int GetKills() const;
		int GetDeath() const;
		void Stun( float duration );
		void DamageLife(float damage);
		void setDeathTimer(float deathTimer);
		bool deathTimerTick(float dt);

		::Oyster::Math::Float3 & GetWeaponMuzzlePosition( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() );
		::Oyster::Math::Float3 & GetWeaponMuzzlePosition( ::Oyster::Math::Float3 &targetMem, const ::Oyster::Physics::ICustomBody::State &state );
		
		void BeginFrame(float dt);
		void EndFrame();
		static Oyster::Physics::ICustomBody::SubscriptMessage PlayerCollisionAfter(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);

	private:
		void Jump();
		void initPlayerData();

		bool UpdateMovement( const ::Oyster::Math::Float4x4 &rotationMatrix, const ::Oyster::Physics::ICustomBody::State &state, bool isGrounded );

	private:
		int teamID;
		Weapon *weapon;
		PLAYER_STATE playerState;
		Oyster::Math::Float3 lookDir;
		float key_forward;
		float key_backward;
		float key_strafeRight;
		float key_strafeLeft;
		float key_jump;


		Oyster::Math::Float rotationUp;

		float deathTimer, stunTimer;

		bool hasTakenDamage, haveRecoveredFromStun;
		Oyster::Math::Float RecentlyAffected;
		PlayerStats playerStats;
		PlayerScore playerScore;

	};
}
#endif