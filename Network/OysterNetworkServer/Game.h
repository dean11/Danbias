#pragma once
#ifndef GAME_H
#define GAME_H
#include "User.h"
#include "ServerInclude.h"
const int MUTEX_COUNT =2;
//Mutex #0=playerPos setGet
//Mutex #1=

//#include "Session.h"

class Game
{
private:
	bool started;
	//ClientToServerUpdateData players[PLAYER_MAX_COUNT];
	User* users[PLAYER_MAX_COUNT];
	int userID[PLAYER_MAX_COUNT];
	bool ready[PLAYER_MAX_COUNT];
	int playerCount;
	
	//Tracks which ship each user has
	int shipID[PLAYER_MAX_COUNT];
	HANDLE mutex[MUTEX_COUNT];
	//::Game::Session *session;
	int sessionID;
public:
	//Will reset all data
	//playerIDs is an array of int which points toward each users connection.
	void setReady(int pid, bool rdy){ready[pid]=rdy;}
	bool allReady(){for (int i=0; i<playerCount; i++){if(ready[i]==false)return false;}return true;}
	void initGame(std::vector<User> players, int nrOfPlayers);
	GameInitData getInitData();
	bool startGame();
	bool isStarted(){return started;}
	Game();
	//Float4x4 getPlayerPos(int id);
	//void setPlayerPos(int id, Float4x4 pos);
	//bool checkMoveValidity(ClientToServerUpdateData plr);
	//ClientToServerUpdateData getPlayerData(int id);
	//void setPlayerData(int id, ClientToServerUpdateData ps);

	int getPlayerCount() {return playerCount;}
	int getUserID(int i) {return userID[i];}

	void initLUA(char* file);
	void update(float dt);
	void addUser(int uid);
	void removeUser(int uid){playerCount--;}
};
#endif