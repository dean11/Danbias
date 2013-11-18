#include "Game.h"
Game::Game()
{
	playerCount=0;
	started=false;
	for (int i=0; i<MUTEX_COUNT; i++)
	{
		mutex[i] = CreateMutex( 
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);             // unnamed mutex

		if (mutex == NULL) 
		{
			printf("CreateMutex error: %d\n", GetLastError());
		}
	}
	for(int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		ready[i]=false;
	}
}
/*bool Game::checkMoveValidity(ClientToServerUpdateData plr)
{

	if (false)
	{
		players[plr.pid]=plr;
		return true;
	} else
	{
		//The package that arrived is an earlier version than the last one. 
		//Ignore the position data, but still check actions and such to make
		//sure that you don't miss a key press.

		//For example, if the fire button is true in this package but false now, 
		//the ship should still shoot once.
		return true;
	}
}
Float4x4 Game::getPlayerPos(int id)
{
	WaitForSingleObject(mutex[0], INFINITE);
	Float4x4 tmp=players[id].position;
	ReleaseMutex(mutex[0]);
	return tmp;
}
void Game::setPlayerPos(int id, Float4x4 pos)
{
	WaitForSingleObject(mutex[0], INFINITE);
	players[id].position=pos;
	ReleaseMutex(mutex[0]);
}
ClientToServerUpdateData Game::getPlayerData(int id)
{
	WaitForSingleObject(mutex[0], INFINITE);
	ClientToServerUpdateData tmp=players[id];
	ReleaseMutex(mutex[0]);
	return tmp;
}
void Game::setPlayerData(int id, ClientToServerUpdateData ps)
{
	WaitForSingleObject(mutex[0], INFINITE);
	players[id]=ps;
	ReleaseMutex(mutex[0]);
}*/
void Game::initGame(std::vector<User> usr, int nrOfPlayers)
{
	/*for (int i=0; i<nrOfPlayers; i++)
	{
		users[i]=&usr[i];
	}*/
	Oyster::Math::Float4x4 initvariable=Oyster::Math::Float4x4::identity;
	initvariable.v[3].x=50;
	for (unsigned int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		initvariable.v[3].x=(Float)200*i;
		//players[i].position=initvariable;
	}
	//players[1].position.m11=0.1f;
	//players[1].position.m22=0.1f;
	//players[1].position.m33=0.1f;
}
GameInitData Game::getInitData()
{
	//Later getInitData will need to receive a user id to set it up 100%.
	//That way, this is the only function that needs to be called in order to connect(or reconnect) to a game.
	GameInitData init;
	
	init.pid=0;
	for (unsigned int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		init.player[i].pid=i;
		init.player[i].teamid=i%2;
		//init.player[i].position=getPlayerPos(i);
		//users[i]->setGame(2);
		//init.players[i]=players[i];
	}
	return init;
}
void Game::addUser(int uid)
{
	userID[playerCount++]=uid;
}
bool Game::startGame()
{
	started=true;
	return started;
}
void Game::update(float dt)
{

}