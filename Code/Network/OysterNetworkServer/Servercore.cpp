#include "SocketServer.h"
#include <fstream>
bool SocketServer::loadMapList(char* maploc)
{
	::std::string workDir;
	::Utility::String::extractDirPath( workDir, maploc, '\\' );

	//maploc is the filename of the list which contains all maps
	//load all map file names into the server, but don't load the maps themselves.
	std::ifstream file;
	file.open(maploc);
	if (!file.is_open())
		return false;
	::std::string str;
	while(!file.eof())
	{
		::std::getline( file, str );
		maps.push_back( workDir + str );
	}

	/*
	maps.push_back("map1test.map");
	maps.push_back("map2 test.map");
	*/
	return true;
}
bool SocketServer::LoadInitData(char* maploc)
{
	std::vector<std::string> cont;
	char* in=new char[100];
	std::ifstream ifs;
	ifs.open(maploc);
	if(!ifs.is_open())
	{
		return false;
	}
	while(!ifs.eof())
	{
		ifs.getline(in, 100);
		cont=splitString(in, '=');
		if (cont.size()==2)
		{
			if(!strcmp("nr_players_per_session", cont[0].c_str()))
			{
				playersPerSessionCount=atoi(cont[1].c_str());
			}
			else if(!strcmp("nr_kills_to_win", cont[0].c_str()))
			{
				killsRequiredPerSession=atoi(cont[1].c_str());
			}
			else if(!strcmp("match_type", cont[0].c_str()))
			{
				//Isn't used
			}
		}

	}
	ifs.close();
}
SocketServer::~SocketServer()
{
	serverTCPConnectionLoopActive=false;
	serverUDPReceiveLoopActive=false;
	serverTCPReceiveLoopActive=false;
	for (int i=0; i<NR_CONNECTTHREADS; i++)
	{
		delete connData[i].buffer;
	}
	for (int i=0; i<NR_SIMULTCPCONNECTS; i++)
	{
		delete tcpData[i].buffer;
	}
	delete sendGameDataBuffer;
	delete sendEffectDataBuffer;
	closeConnection();
}
void SocketServer::startGameCreateLoop(int delay)
{
	lobbyActive=false;
	DEBUGCTR=0;
	if(!serverGameCreationActive)
	{
		ThreadArguments tr;
		tr.ptr=this;
		tr.threadID=delay;
		serverGameCreationActive=true;
		gameCreateHandle=CreateThread(
			NULL, //Choose default security
			0, //Default stack size
			(LPTHREAD_START_ROUTINE)&activateServerGameLoop,
			//Routine to execute
			(LPVOID) &tr, //Thread parameter
			0, //Immediately run the thread
			0 //Thread Id
			);
		if (gameCreateHandle == NULL)
		{
			printf("Game creation thread ERROR");
		}
		else
		{
			printf("Game creation thread successful\n");
			Sleep(100);
		}
	}
}
void SocketServer::stopGameCreateLoop()
{
	serverGameCreationActive=false;
	WaitForSingleObject(gameCreateHandle, INFINITE);
	printf("Game Creation thread ended.\n");
}
DWORD SocketServer::activateServerGameLoop(ThreadArguments* tra)
{	
	srand((unsigned int)(time(0)));
	(tra->ptr)->serverGameCreationLoop(tra->threadID);
	return 0;
}
bool SocketServer::serverGameCreationLoop(int delay)
{ // TODO: Mem access Violoation Crash 0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff ... delay = -858993460
	//Mem access violation in a thread can also be caused by failure from something else instead of it, 
	//it still breaks at header even if, for example, server->load or lobby.startLobbyCountdown breaks it
	//If you get an error here, make sure that isn't the problem.
	int count;
	while(serverGameCreationActive)
	{
		if (nrActiveSessions==0)
		{
			count=0;
			for (unsigned int i=0; i<users.size(); i++)
			{
				if (users[i].getState()==ONLINE_QUEUEING)
				{
					count++;
				}
			}
			if (count>=playersPerSessionCount)
			{
				games.resize(1);
				//lobby.resize(games.size()+1);
				session =new  GameLogic::Session();
				lobby = Lobby();
				timer.resize(1);
				timeTillUpdate.resize(1);
				timeTillUpdate[0]=GAME_UPDATEDELAY;
				updateCount.resize(1);
				updateCount[0]=0;
				int curID=(int)games.size()-1;
				int mapid=rand()%maps.size();
				session->setNrPlayers(playersPerSessionCount);
				session->setKillsRequired(killsRequiredPerSession);
				session->load(maps[mapid]);
				printf("Map nr %d loaded, name %s.\n",mapid, maps[mapid].c_str());
				count=0;
				for (unsigned int i=0; count<playersPerSessionCount && i<users.size(); i++)
				{
					if (users[i].getState()==ONLINE_QUEUEING)
					{
						//Set to INLOBBY and send lobby data, then start a lobby
						lobby.addUser(users[i], i);
						users[i].setState(ONLINE_INGAME);
						games[curID].addUser(i);
						users[i].setGame(curID);
						session->accessPlayer(i).spawn();
						count++;
					}
				}
				lobbyActive=true;
				sendLobbyInitData(curID);
				lobby.startLobbyCountdown(LOBBY_WAIT_TIME);
				sendRenderData(curID);
				//return true;
			}
			if(lobbyActive)
			{
				for (int i=0; i<1; i++)
				{
					float ttimer=lobby.timeLeft();
					if (ttimer==0)
					{
						printf("Starting game.\n");
						games[i].initGame(users,playersPerSessionCount);
						sendInitData(i);
						nrActiveSessions++;
						lobbyActive=false;
						//serverGameCreationActive=false;
					}
				}
			}
		}
		Sleep(delay);
	}
	printf("Maximum server count reached, shutting down the sever creation thread.\n");
	return false;
}
SocketServer::SocketServer()
{
	UDPSocket = INVALID_SOCKET;
	nrActiveSessions=0;
	serverGameCreationActive=false;
	serverTCPConnectionLoopActive=false;
	serverTCPReceiveLoopActive=false;
	serverUDPReceiveLoopActive=false;
	killsRequiredPerSession=10;
	playersPerSessionCount=1;
	LoadInitData("../ServerData.dat");
	//---------------------------------------------
	// Set up the port and IP of the server
	//Port starts up as a different one from when UDPSocketected, it changes once the server has exchanged some info with the client
	UDPRecvAddr.sin_family = AF_INET;
	UDPRecvAddr.sin_port = htons(UDPRecvPort);
	UDPRecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	sessionEvents=std::vector<Event::GameEvent*>(0);
	sessionEffects=std::vector<Network::EffectData>(0);
	TCPRecvAddr.sin_family = AF_INET;
	TCPRecvAddr.sin_port = htons(TCPRecvPort);
	TCPRecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	addrSize=sizeof(sockaddr_in);
	for (int i=0; i<NR_CONNECTTHREADS; i++)
	{
		connData[i].buffer=new char[256];
		connData[i].bufLen=256;
		ZeroMemory(connData[i].buffer,sizeof(connData[i].buffer));
		connData[i].dataSize=0;
		//connData[i].srcAddr
		memcpy(&connData[i].srcAddr, &UDPRecvAddr,addrSize);
	}
	for (int i=0; i<NR_SIMULTCPCONNECTS; i++)
	{
		tcpData[i].buffer=new char[256];
		tcpData[i].bufLen=256;
		ZeroMemory(tcpData[i].buffer,sizeof(tcpData[i].buffer));
		tcpData[i].dataSize=0;
		memcpy(&connData[i].srcAddr, &TCPRecvAddr,addrSize);
	}
	sendGameDataBufferSize=SERVER_PLAYER_DATA_SIZE*playersPerSessionCount+1;
	sendGameDataBuffer=new char[sendGameDataBufferSize];
	sendGameDataBuffer[0]=1;
	sendEffectDataBuffer=new char[sizeof(Network::EffectData)+1];
	sendEffectDataBuffer[0]=7;

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
	}
	else
	{
		//Init sockets
		setupStatus=initTCPSocket();
		if(setupStatus)
			setupStatus=initUDPSocket();
	}
}
bool SocketServer::startThreads()
{
	//ThreadArguments tra1[NR_SIMULTCPCONNECTS];
	ThreadArguments tra2[NR_CONNECTTHREADS];
	//for (int i=0; i< NR_SIMULTCPCONNECTS; i++) 
	//{
	//	tra1[i].ptr=this;
	//	tra1[i].threadID=i;
	//	//printf("i - %d\n",i);
	//	
	//	tcpDataHandle[i]=CreateThread(
	//		NULL, //Choose default security
	//		0, //Default stack size
	//		(LPTHREAD_START_ROUTINE)&activateTCPConnectLoop,
	//		//Routine to execute
	//		(LPVOID) &tra1[i], //Thread parameter
	//		0, //Immediately run the thread
	//		0 //Thread Id
	//		);
	//	if (tcpDataHandle[i] == NULL)
	//	{
	//		printf("Error Creating TCP Thread#: %d\n",i);
	//		return(false);
	//	}
	//	else
	//	{
	//		printf("Successfully created TCP thread #: %d\n", i);
	//		Sleep(100);
	//	}
	//}
	for (int i=0; i< NR_CONNECTTHREADS; i++) 
	{
		tra2[i].ptr=this;
		tra2[i].threadID=i;
		//printf("i - %d\n",i);
		udpDataHandle[i]=CreateThread(
			NULL, //Choose default security
			0, //Default stack size
			(LPTHREAD_START_ROUTINE)&activateUDPReceiveLoop,
			//Routine to execute
			(LPVOID) &tra2[i], //Thread parameter
			0, //Immediately run the thread
			0 //Thread Id
			);
		if (udpDataHandle[i] == NULL)
		{
			printf("Error Creating UDP Thread#: %d\n",i);
			return(false);
		}
		else
		{
			printf("Successfully created UDP thread #: %d\n", i);
			Sleep(100);
		}
	}
	return true;
}
bool SocketServer::sendData(int uid, const char* data, int size)
{
	//---------------------------------------------
	// Send a datagram to a user
	//uid -1 = broadcast message
	if (uid<0)
	{
		for (unsigned int i=0; i<users.size(); i++)
		{
			iResult = sendto(UDPSocket, data, size, 0, (SOCKADDR *) & users[i].getAddr(), addrSize);
			if (iResult == SOCKET_ERROR) 
			{
				wprintf(L"UDP sendData(-1) sendto failed with error: %d\n", WSAGetLastError());
				closesocket(UDPSocket);
				WSACleanup();
				return false;
			}
		}
	}
	else
	{
		if((unsigned)uid>=users.size())
		{
			//User doesn't exist
			printf("UDP sendData(%d) sendto failed because the specified user does not exist\n", uid);
		}
		else
		{
			iResult = sendto(UDPSocket, data, size, 0, (SOCKADDR *) & users[uid].getAddr(), addrSize);
			if (iResult == SOCKET_ERROR) 
			{
				wprintf(L"UDP sendData(%d) sendto failed with error: %d\n", uid, WSAGetLastError());
				closesocket(UDPSocket);
				WSACleanup();
				return false;
			}
		}
	}
	return true;
}
bool SocketServer::sendKeyFrameData(int size, const char* data)
{
	for (int i=0; i<playersPerSessionCount; i++)
	{
		iResult = sendto(UDPSocket, data, size+1, 0, (SOCKADDR *) & users[games[0].getUserID(i)].getAddr(), addrSize);
		if (iResult == SOCKET_ERROR) 
		{
			wprintf(L"UDP keyFrameData sendto failed with error: %d\n", WSAGetLastError());
			closesocket(UDPSocket);
			WSACleanup();
			return false;
		}
	}
	return true;
}

bool SocketServer::checkConnection(int userID)
{
	char* message="\3testmessage";
	int count=
		sendto(
		UDPSocket, 
		message, 
		(int)strlen(message), 
		0, 
		(SOCKADDR *) & users[userID].getAddr(), 
		addrSize);
		
	if (count == SOCKET_ERROR) 
	{
		wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		return false;
	} 
	else if (count==0)
	{
		wprintf(L"Disconnected.\n");
		return false;
	}
	return true;
}


bool SocketServer::closeConnection()
{
	//---------------------------------------------
	// When the application is finished sending, close the sockets.
	setupStatus=false;
	wprintf(L"Finished sending. Closing socket.\n");
	iResult = closesocket(UDPSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closeUDPsocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	iResult = closesocket(TCPSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closeTCPsocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	//---------------------------------------------
	// Clean up and quit.
	wprintf(L"Exiting.\n");
	WSACleanup();
	return true;
}