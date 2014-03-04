//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef GAMEMODETYPE_H
#define GAMEMODETYPE_H
#include "Player.h"
#include "DynamicArray.h"

namespace GameLogic
{
	class GameModeType
	{

		public:
			enum GameMode
			{
				FREE_FOR_ALL
			};
			struct EndConditions
			{
				float timer; 
				float endTimer;
				int killCount;

				EndConditions()
				{
					this->endTimer = 30000;
					this->killCount = 5;
					this->timer = 0;
				}
			};
			GameModeType(void);
			~GameModeType(void);

			void initGameMode( EndConditions end );
			void Update( float deltaTime);
			bool TimeExit();
			int EndConditionMet( Utility::DynamicMemory::DynamicArray<Player*>& players );

		private:
			EndConditions endConditions;
	};

}
#endif