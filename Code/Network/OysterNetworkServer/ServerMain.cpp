#include <ctime>
#include "SocketServer.h"
#include "ServerTimer.h"
#include <iostream>
#include <string>
#include <stdio.h>
//#ifdef WINDOWS
#include <direct.h>
#include "ServerInclude.h"
#define GetCurrentDir _getcwd
//#else
	//For other OS than windows; can't be found on 
	//all windows setups so it's commented for now
	//#include <unistd.h>
	//#define GetCurrentDir getcwd
 //#endif

char* getCurDir()
{
	char* cCurrentPath;
	cCurrentPath=new char[FILENAME_MAX];
	int test=sizeof(cCurrentPath);
	if (!GetCurrentDir(cCurrentPath, FILENAME_MAX))
	{
		return "ERROR";
	}
	cCurrentPath[FILENAME_MAX - 1] = '\0';
	return cCurrentPath;
}
int main(int argc, char *argv[])
{
	srand((unsigned int)time(0));
	::Oyster::Game::MoveAble::setDiscreteTimeSlice( GAME_UPDATEDELAY );

	SocketServer server;
	server.loadMapList("..\\Content\\Maplist.txt");
	while (!server.isReady());
	server.startThreads();
	GameLogic::Object::init("NOT_IMPLEMENTED");
	server.startGameCreateLoop(50);
	while(true)
	{
		server.updateServers();
	}
	server.closeConnection();
	return 0;
}