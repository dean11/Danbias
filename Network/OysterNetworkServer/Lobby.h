#include "ServerInclude.h"
#include "User.h"
#ifndef LOBBY_H
#define LOBBY_H
class Lobby
{
private:
	int nrUsers;
	int userID[PLAYER_MAX_COUNT];
	ServerTimer countdownTimer;
	float countdownLimit;
	LobbyUserStruct userData[PLAYER_MAX_COUNT];
	bool timerStarted;
	HANDLE timerMutex;
public:
	Lobby();
	void addUser(User usr, int i);
	int getUserID(int i) const {return userID[i];}
	int getNrPlayers() const {return nrUsers;}
	void removeUser();
	void updateUserData(LobbyUserStruct);
	LobbyInitData getLobbyInitData();
	void startLobbyCountdown(float seconds);
	float timeLeft();

};
#endif