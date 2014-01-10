#ifndef GAME_H
#define GAME_H

#include "GameLogicStates.h"
namespace GameLogic
{
	class Game
	{
	
	public:
		struct PlayerData
		{
			int playerID;
			int teamID;

			PlayerData()
			{
				playerID = 0;
				teamID = 0;
			}

			PlayerData(int playerID,int teamID)
			{
				this->playerID = playerID;
				this->teamID = teamID;
				
			}

			~PlayerData()
			{
			
			}
		};

	public:
		Game(void);
		~Game(void);

		/********************************************************
		* Moves the chosen player based on input
		* @param playerID: ID of the player you want to recieve the message
		* @param movement: enum value on what kind of action is to be taken
		********************************************************/
		void MovePlayer(int playerID, const PLAYER_MOVEMENT &movement);
		
		/********************************************************
		* Uses the chosen players weapon based on input
		* @param playerID: ID of the player you want to recieve the message
		* @param Usage: enum value on what kind of action is to be taken
		********************************************************/
		void PlayerUseWeapon(int playerID, const WEAPON_FIRE &Usage);

		/********************************************************
		* Gets a specific players position
		* @param playerID: ID of the player whos position you want
		********************************************************/
		void GetPlayerPos(int playerID);

		/********************************************************
		* Gets the position of all players currently in the game
		********************************************************/
		void GetAllPlayerPos();

		/********************************************************
		* Creates a player and returns PlayerData containing ID of the player
		********************************************************/
		PlayerData CreatePlayer();

		/********************************************************
		* Creates a team
		********************************************************/
		void CreateTeam();

		/********************************************************
		* Runs a update of the gamelogic and physics
		********************************************************/
		void NewFrame();
	
	
	private:
		struct PrivateData;
		PrivateData *myData;

	};	
}


#endif
