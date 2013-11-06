#include "Lobby.h"

Lobby::Lobby()
{
	timerStarted=false;
	nrUsers=0;
	timerMutex = CreateMutex( 
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);             // unnamed mutex

	if (timerMutex == NULL) 
	{
		printf("CreateMutex error: %d\n", GetLastError());
	}
	for(int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		userData[i].pid=i;
		userData[i].shipID=0;
		userData[i].usrName[0]='\0';
		//userData[i].usrName="Player";
		//userData[i].usrName+=(char)i;
	}
}
void Lobby::removeUser()
{
}
void Lobby::addUser(User usr, int i)
{
	userID[nrUsers]=i;
	userData[nrUsers].setName(usr.getUsername().c_str());
	//userData[nrUsers].shipID=1;
	nrUsers++;
}
void Lobby::updateUserData(LobbyUserStruct data)
{
	userData[data.pid]=data;
}
LobbyInitData Lobby::getLobbyInitData()
{
	LobbyInitData data;
	data.playerCount=nrUsers;
	for(int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		data.players[i]=userData[i];
	}
	return data;
}
void Lobby::startLobbyCountdown(float seconds)
{
	WaitForSingleObject(timerMutex, INFINITE);
	countdownLimit=seconds;
	countdownTimer.reset();
	countdownTimer.start();
	timerStarted=true;
	ReleaseMutex(timerMutex);
}
float Lobby::timeLeft()
{
	WaitForSingleObject(timerMutex, INFINITE);
	countdownTimer.tick();
	if (!timerStarted)
		return -1;
	else
	{
		float timeLeft=countdownLimit-countdownTimer.getGameTime();
		if(timeLeft>0)
			return timeLeft;
		else
			return 0;
	}
	ReleaseMutex(timerMutex);
}