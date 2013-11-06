#include "NetworkIncludes.h"
#include "NetworkConstants.h"
#ifndef NET_EVT_STRUCTS_H
#define NET_EVT_STRUCTS_H
struct Score
{
	int id;//Leaderboard var
	int kills;
	int deaths;
	int teamkills;
	Score(){id=0;kills=0;deaths=0;teamkills=0;}
	Score& operator+=(const Score add)
	{
		id+=add.id;
		kills+=add.kills;
		deaths+=add.deaths;
		teamkills+=add.teamkills;
		return *this;
	}
};
namespace EventStruct
{
	struct BulletCreatedStruct
	{
		Float3 position;
		Float3 head;
		int owner;
	};
	struct BulletHitStruct
	{
		int hitTarget;
		int attackingTarget;
	};
	struct ShipSpawnedStruct
	{
		Float3 position;
		int playerID;
	};
	struct ShipDestroyedStruct
	{
		int playerID;
		int killerID;
		Score scoreboard[PLAYER_MAX_COUNT];
	};
	struct ScoreUpdateStruct
	{
		Score scoreboard[PLAYER_MAX_COUNT];
	};
	struct GameEndedStruct
	{
		int winningTeam;
		Score scoreboard[PLAYER_MAX_COUNT];
	};
}
#endif