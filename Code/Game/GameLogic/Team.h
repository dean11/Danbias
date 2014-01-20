//////////////////////////////////////////////////
//Created by Erik of the GameLogic team
//////////////////////////////////////////////////

#ifndef TEAM_H
#define TEAM_H

#include "Player.h"
#include "DynamicArray.h"

namespace GameLogic
{

	class Team
	{
	public:
		Team(void);
		Team(int teamSize);
		~Team(void);

		/********************************************************
		* Gets a player in the team
		* @param playerID: Arrayposition of the player you want to get
		********************************************************/
		Player*  GetPlayer(int playerID);

		/********************************************************
		* Adds a player to the team
		* @param player: Player that are to be added
		********************************************************/
		bool AddPlayer(Player *player);

	private:
		Utility::DynamicMemory::DynamicArray<Player*> players;
		int teamSize;
	};
}
#endif

