/////////////////////////////////////////////////////////////////////
// Created by [Erik Persson] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef GAME_H
#define GAME_H

#include "GameLogicDef.h"
#include "GameLogicStates.h"

namespace GameLogic
{
	class Player;
	class Game
	{
	
	public:
		struct PlayerData
		{
		private:
			friend class Game;
			Player *player;
			

		public:

			int playerID;
			int teamID;


			PlayerData();

			PlayerData(int playerID,int teamID);


			~PlayerData()
			{
			
			}

			/********************************************************
			* Moves the chosen player based on input
			* @param playerID: ID of the player you want to recieve the message
			* @param movement: enum value on what kind of action is to be taken
			********************************************************/
			void MovePlayer(const PLAYER_MOVEMENT &movement);


		};

	public:
		Game(void);
		~Game(void);

		
		
		
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
