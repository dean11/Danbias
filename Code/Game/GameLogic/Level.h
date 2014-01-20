//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef LEVEL_H
#define LEVEL_H
#include <string>

#include "Player.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "GameMode.h"
#include "Player.h"
#include "PhysicsAPI.h"
#include "TeamManager.h"
#include "DynamicArray.h"

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
		void InitiateLevel(std::string levelPath);
		void Level::InitiateLevel(float radius);

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


		/********************************************************
		* Respawns a player on a random teammate
		* @param player: The player you want to respawn
		********************************************************/
		void RespawnPlayer(Player *player);

	private:
		TeamManager teamManager;
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<StaticObject>> staticObjects;
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<DynamicObject>> dynamicObjects;
		GameMode gameMode;
		Utility::DynamicMemory::SmartPointer<Oyster::Physics::ICustomBody> rigidBodyLevel;

	};

}
#endif