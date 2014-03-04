//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef LEVEL_H
#define LEVEL_H
#include <string>

#include "Player.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "GameModeType.h"
#include "SpawnPoint.h"

#include "Player.h"
#include "PhysicsAPI.h"
#include "TeamManager.h"
#include "DynamicArray.h"
#include "LevelLoader.h"

#include "PickupSystem\PickupSystem.h"
#include "PickupSystem\PickupHealth.h"

const int DEATH_TIMER = 3;
namespace GameLogic
{

	class Level
	{

	public:
		Level(void);
		~Level(void);

		/********************************************************
		* Initiates a level for players to play on
		* @param levelPath: Path to a file that contains all information on the level
		********************************************************/
		bool InitiateLevel(std::wstring levelPath);
		bool InitiateGameMode(float maxTimeSec, int endKillScore);
		Oyster::Physics::ICustomBody* InitRigidBodyCube( const ObjectHeader* obj);
		Oyster::Physics::ICustomBody* InitRigidBodySphere( const ObjectHeader* obj);
		Oyster::Physics::ICustomBody* InitRigidBodyMesh( const ObjectHeader* obj);
		

		Object* CreateGameObj(ObjectHeader* obj, Oyster::Physics::ICustomBody* rigidBody);
		/********************************************************
		* Creates a team in the level
		* @param teamSize: The size of the team you want to create
		********************************************************/
		void CreateTeam(int teamSize);

		/********************************************************
		* Adds a player to a team
		* @param player: The player you want to add to the team
		* @param teamID: ArrayPos of the team you want to add the player to
		********************************************************/
		void AddPlayerToTeam(Player *player, int teamID);
		void AddPlayerToGame(Player *player);
		void RemovePlayerFromGame(Player *player);

		/********************************************************
		* Respawns a player on a random teammate
		* @param player: The player you want to respawn
		********************************************************/
		void RespawnPlayer(Player *player);

		/********************************************************
		* Collision function for level, this is to be sent to physics through the subscribe function with the rigidbody
		* Will be called when the physics detect a collision
		* @param rigidBodyLevel: physics object of the level
		* @param obj: physics object for the object that collided with the level
		********************************************************/
		static Oyster::Physics::ICustomBody::SubscriptMessage LevelCollisionAfter(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);
		
		void Update(float deltaTime);

		int getNrOfDynamicObj();
		Object* GetObj( int ID ) const;

		static void PlayerDied( Player* player );
		static void PhysicsOnMoveLevel(const Oyster::Physics::ICustomBody *object);
		
		Utility::DynamicMemory::DynamicArray<Player*>			GetPlayers();
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<StaticObject>>	GetStaticObjects();
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<DynamicObject>>	GetDynamicObject();

	private:
		Utility::DynamicMemory::DynamicArray<Player*> playerObjects;
		TeamManager teamManager;
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<StaticObject>> staticObjects;
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<DynamicObject>> dynamicObjects;
		GameModeType gameMode;
		int objIDCounter; 
		SpawnPoint spawnPoints;
		PickupSystem pickupSystem;

	};

}
#endif