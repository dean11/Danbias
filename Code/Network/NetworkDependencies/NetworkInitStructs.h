#ifndef NET_INIT_STRUCTS_H
#define NET_INIT_STRUCTS_H
#include "NetworkIncludes.h"
#include "NetworkConstants.h"
struct PlayerInitStruct
{
	INT8 pid;
	int teamid;
	Oyster::Math::Float4x4 position;
	PlayerInitStruct()
	{
		pid=0;
		//position=Oyster::Math::Float4x4::identity;
	}
};

struct GameInitData
{
	INT8 pid;
	//std::string playerNames[PLAYER_MAX_COUNT];
	PlayerInitStruct player[PLAYER_MAX_COUNT];
};

struct LobbyUserStruct
{
	INT8 pid;
	INT8 shipID;
	char usrName[15];
	LobbyUserStruct()
	{
		pid=0;
		shipID=0;
		usrName[0]='\0';
	}
	void setName(const char* n)
	{
		strcpy_s(usrName, n);
	}
	int size()
	{
		int sz=sizeof(pid);
		sz+=sizeof(shipID);
		int tmp=(int)strlen(usrName);
		sz+=(int)strlen(usrName);
		return sz;
	}
};
struct LobbyInitData
{
	INT8 pid;
	INT8 playerCount;
	int timer;
	LobbyUserStruct players[PLAYER_MAX_COUNT];
	LobbyInitData()
	{
		pid=0;
		for (int i=0; i<PLAYER_MAX_COUNT; i++)
		{
			players[i].pid=i;
		}
	}
	int size()
	{
		int sz=sizeof(pid);
		for (int i=0; i<PLAYER_MAX_COUNT; i++)
			sz+=players[i].size();
		return sz;
	}
};
#endif