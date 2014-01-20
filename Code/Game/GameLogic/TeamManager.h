//////////////////////////////////////////////////
//Created by Erik of the GameLogic team
//////////////////////////////////////////////////

#ifndef TEAMMANAGER_H
#define TEAMMANAGER_H

#include "Player.h"
#include "Team.h"
#include <DynamicArray.h>

namespace GameLogic
{

	class TeamManager
	{
		public:
			TeamManager(void);
			TeamManager(int maxNrOfTeams);
			~TeamManager(void); 

			/********************************************************
			 * Respawns the chosen player on a random teammember
			 * @param player: Pointer to the player you want to respawn
			 ********************************************************/
			void RespawnPlayerRandom(Player *player);

			/********************************************************
			 * Creates a team that can hold players
			 * @param teamSize: The number of players that can fit in this team 
			 ********************************************************/
			void CreateTeam(int teamSize);


			/********************************************************
			 * Removes the chosen team
			 * @param teamID: This is the array ID of the team you want to remove
			 ********************************************************/
			void RemoveTeam(int teamID);


			/********************************************************
			 * Adds a player to the teamID given
			 * @param player: Player that you want to add to a team
			 * @param teamID: The team you want to place the player in
			 ********************************************************/
			bool AddPlayerToTeam(Player *player ,int teamID);

			/********************************************************
			 * Adds a player to the team with the least amount players
			 * @param player: Player that you want to add to a team
			 ********************************************************/
			bool AddPlayerToTeam(Player *player);

			/********************************************************
			 * Checks if the team exists or not, returns true if yes
			 * @param teamID: ID of the team that are to exist or not
			 ********************************************************/
			bool IsValidTeam(int teamID);

		private:
			Utility::DynamicMemory::DynamicArray<Team*> teams;
			int maxNrOfTeams;
	};
}
#endif