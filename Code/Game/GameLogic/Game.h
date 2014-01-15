/////////////////////////////////////////////////////////////////////
// Created by [Erik Persson] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef GAME_H
#define GAME_H

#include "GameLogicDef.h"
#include "GameLogicStates.h"
#include <OysterMath.h>

namespace GameLogic
{
	class Player;
	class DANBIAS_GAMELOGIC_DLL Game
	{
	
	public:
		struct DANBIAS_GAMELOGIC_DLL PlayerData
		{
		private:
			friend class Game;
			Player *player;
			PlayerData();
			PlayerData(int playerID,int teamID);
			~PlayerData();
			
		public:
			/********************************************************
			* Moves the chosen player based on input
			* @param playerID: ID of the player you want to recieve the message
			* @param movement: enum value on what kind of action is to be taken
			********************************************************/
			void Move(const PLAYER_MOVEMENT &movement);

			/********************************************************
			* Uses the chosen players weapon based on input
			* @param playerID: ID of the player you want to recieve the message
			* @param Usage: enum value on what kind of action is to be taken
			********************************************************/
			void UseWeapon(int playerID, const WEAPON_FIRE &Usage);

			/********************************************************
			* Gets players position
			* @param playerID: ID of the player whos position you want
			********************************************************/
			Oyster::Math::Float3 GetPosition();

			/********************************************************
			* Gets players current orientation
			* @param playerID: ID of the player whos position you want
			********************************************************/
			Oyster::Math::Float4x4 GetOrientation();

			/********************************************************
			* Check player state
			* @return The current player state
			********************************************************/
			PLAYER_STATE GetState() const;

			/***/
			int GetID() const;

			/***/
			int GetTeamID() const;
		};

	public:
		Game(void);
		~Game(void);

		/********************************************************
		* Gets the position of all players currently in the game
		********************************************************/
		void GetAllPlayerPositions() const;

		/********************************************************
		* Creates a player and returns PlayerData containing ID of the player
		********************************************************/
		PlayerData* CreatePlayer();

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
