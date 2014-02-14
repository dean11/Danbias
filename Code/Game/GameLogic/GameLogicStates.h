#ifndef GAMELOGICSTATES_H
#define GAMELOGICSTATES_H
#include "OysterMath.h"

namespace GameLogic
{
	enum PLAYER_MOVEMENT
	{
		PLAYER_MOVEMENT_FORWARD = 0,
		PLAYER_MOVEMENT_BACKWARD = 1,
		PLAYER_MOVEMENT_LEFT = 2,
		PLAYER_MOVEMENT_RIGHT = 4,
		PLAYER_MOVEMENT_JUMP = 8,
	};
	enum PLAYER_STATE
	{
		PLAYER_STATE_JUMPING = 0,
		PLAYER_STATE_WALKING = 1,
		PLAYER_STATE_IDLE = 2,
		PLAYER_STATE_DEAD = 4,
		PLAYER_STATE_INVALID = 8,
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
		WEAPON_STATE_FIRING = 0,
		WEAPON_STATE_IDLE = 1,
		WEAPON_STATE_RELOADING = 2,
	};

	struct forcePushData
	{
		Oyster::Math::Float3 pushForce;
	};

	struct ExplosionData
	{
		Oyster::Math::Float3 pushForce;
		Oyster::Math::Float3 ShreddingDamage;
	};


	
};

#endif