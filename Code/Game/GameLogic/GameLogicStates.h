#ifndef GAMELOGICSTATES_H
#define GAMELOGICSTATES_H

#include "GameLogicDef.h"

namespace GameLogic
{
	class DANBIAS_GAMELOGIC_DLL PLAYER_STATE
	{
		public:
			PLAYER_STATE()
			{

			}
			PLAYER_STATE(int value)
			{
				this->value = value;
			}

			static const int PLAYER_STATE_JUMPING = 0;
			static const int PLAYER_STATE_WALKING = 1;
			static const int PLAYER_STATE_IDLE = 2;
			int value;

	};
	class DANBIAS_GAMELOGIC_DLL PLAYER_MOVEMENT
	{
		public:
		PLAYER_MOVEMENT()
		{

		}
		PLAYER_MOVEMENT(int value)
		{
			this->value = value;
		}
		static const int PLAYER_MOVEMENT_FORWARD = 0;
		static const int PLAYER_MOVEMENT_BACKWARD = 1;
		static const int PLAYER_MOVEMENT_LEFT = 2;
		static const int PLAYER_MOVEMENT_RIGHT = 4;
		static const int PLAYER_MOVEMENT_JUMP = 8;
		int value;
	};

	class DANBIAS_GAMELOGIC_DLL OBJECT_TYPE
	{
		public:
		OBJECT_TYPE()
		{

		}
		OBJECT_TYPE(int value)
		{
			this->value = value;
		}
		static const int OBJECT_TYPE_PLAYER = 0;
		static const int OBJECT_TYPE_BOX = 1;
		static const int OBJECT_TYPE_UNKNOWN = 2;

		public: int value;
	};

	enum WEAPON_FIRE
	{
		WEAPON_USE_PRIMARY_PRESS = 0,
		WEAPON_USE_PRIMARY_RELEASE = 1,
		WEAPON_USE_SECONDARY_PRESS = 2,
		WEAPON_USE_SECONDARY_RELEASE = 4,
		WEAPON_USE_UTILLITY_PRESS = 8,
		WEAPON_USE_UTILLITY_RELEASE = 16,
	};

	enum WEAPON_STATE
	{
		WEAPON_STATE_FIREING = 0,
		WEAPON_STATE_IDLE = 1,
		WEAPON_STATE_RELOADING = 2,
	};

	
};

#endif