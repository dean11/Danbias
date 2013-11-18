#include "SocketServer.h"



void SocketServer::parseReceivedData(int threadID/*char* data, int size*/)
{
	bool test=false;
	for(unsigned int i=0; i<users.size(); i++)
	{
		if(memcmp(&connData[threadID].srcAddr, &users[i].getAddr(), sizeof(sockaddr_in)) != 0)
		{
			//User i has not sent the data.
			test=false;
		}
		else
		{
			//Found the user which sent the data
			test=true;
			switch (connData[threadID].buffer[0])
			{
			case 1://It's data
				if(users[i].isIngame()) parseData(i, users[i].getGame(), threadID);
				break;
			case 2://It's a user-entered command
				parseServercommand(i, threadID);
				break;
			case 3://It's a chat message
				parseMessage(i, threadID);
				break;
			}
			break;
		}
	}

	if(!test)
	{
		//User does not exist yet
		//This is temporary until i have a proper login process in place
		addUser(threadID);
	}
}

void SocketServer::addUser(int threadID)
{
	printf("UDP adding user.\n");
	User usr=User((int)users.size(),connData[threadID].srcAddr);
	connData[threadID].buffer[connData[threadID].dataSize]='\0';
	usr.setUsername(connData[threadID].buffer);
	users.push_back(usr);
	sendData(((int)users.size())-1, "\4connected",10);
	std::string asd=users[users.size()-1].getUsername();
	printf("Username:%s, IP:%s\n",users[users.size()-1].getUsername().c_str(), inet_ntoa(users[users.size()-1].getAddr().sin_addr));
}
void SocketServer::AddUser(ConnThreadData* data)
{
	printf("TCP adding user.\n");
	User usr=User((int)users.size(),data->srcAddr);
	data->buffer[data->dataSize]='\0';
	usr.setUsername(data->buffer);
	users.push_back(usr);
	sendData(((int)users.size())-1, "\4connected",10);
	std::string asd=users[users.size()-1].getUsername();
	printf("Username:%s, IP:%s\n",users[users.size()-1].getUsername().c_str(), inet_ntoa(users[users.size()-1].getAddr().sin_addr));
}
void SocketServer::removeUser(int id)
{
	games[users[id].getGame()].removeUser(id);
	users.erase(users.begin()+id);
}
void SocketServer::parseServercommand(int pid, int threadID)
{
	connData[threadID].buffer[connData[threadID].dataSize]='\0';
	wprintf(L"User %d sent a server command.\n", pid);
	printf("The command is the following:%s.\n", connData[threadID].buffer+1);
	std::vector<std::string> list=splitString(connData[threadID].buffer+1, ' ');
	bool validcommand=false;
	if(list.size()==0)
	{
		//Ignore case 1, to avoid vector subscript out of range errors
	}
	//First variable: Command
	else if(!list[0].compare(" "))
	{
		//Add rest ignore cases here
	}
	else if(!list[0].compare("help"))
	{
		validcommand=true;
	}
	//else if(!list[0].compare("startgame"))
	//{
		//validcommand=true;
		//Do more than just sending init data here
		//sendInitData();
	//}
	else if (!list[0].compare("exit"))
	{
		validcommand=true;
		//User #pid needs to be removed here, and data needs to be sorted accordingly.
	}
	else if (!list[0].compare("qst"))
	{
		validcommand=true;
		if (users[pid].getState()==ONLINE)
		{
			sendData(pid, "\4qst",4);
			users[pid].setState(ONLINE_QUEUEING);
		}
	}
	else if (!list[0].compare("qed"))
	{
		validcommand=true;
		if (users[pid].getState()==ONLINE_QUEUEING)
		{
			sendData(pid, "\4qed",4);
			users[pid].setState(ONLINE);
		}
	}
	else if (!list[0].compare("rdy"))
	{
		if (users[pid].getState()==ONLINE_INGAME)
		{
			games[users[pid].getGame()].setReady(pid, true);
		}
	}
	else if (!list[0].compare("dc"))
	{
		validcommand=true;
		printf("User %s (ID:%d) has disconnected.",users[pid].getUsername().c_str(), pid);
		users[pid].setState(OFFLINE);
		removeUser(pid);
		//Tell games that he might be in here taht he's down
		//users.erase(users.begin()
	}
	else if((!list[0].compare("w")||!list[0].compare("whisper")||!list[0].compare("msg")) && list.size()>2)
	{
		validcommand=true;
		for(unsigned int i=0; i<users.size(); i++)
		{
			//Second variable: Target user
			if (!list[1].compare(users[i].getUsername()))
			{
				//Other variables: Text message. 
				//The +3 is for the 2 spaces and the first /. Calculating the start pos of the message.
				int startloc=(int)(list[0].length()+list[1].length())+3;
				//std::string msg="\3[Whisper] "+users[pid].getUsername()+":"+(connData[threadID].buffer+startloc);
				//msg+=users[pid].getUsername()
				//sendData(i,msg.c_str(), msg.length());
			}
		}
	}
	else if(!list[0].compare("setname"))
	{
		if(list.size()>1)
		{
			users[pid].setUsername(list[1]);
			//list[1]="\3Your username has been changed to "+list[1];
			//sendData(pid,list[1].c_str(), list[1].length());
			validcommand=true;
		}
	}
	if(!validcommand)
	{
		int a=0;
		//sendData(pid, "\3Invalid server command.", 24);
		//Tell user that the server command was invalid
	}
}
void SocketServer::parseData(int pid, int gid, int threadID)
{
	memcpy(&connData[threadID].tmpdata,connData[threadID].buffer+1,CLIENT_PLAYER_DATA_SIZE);
	//No old packets
	if (users[pid].getLastUpdate()<connData[threadID].tmpdata.updateCount)
	{
		users[pid].setLastUpdate(connData[threadID].tmpdata.updateCount);
		users[pid].setLastUpdateData(connData[threadID].tmpdata);
		ControlPlayer(session->accessPlayer(pid),connData[threadID].tmpdata);
	}
}
void SocketServer::parseMessage(int pid, int threadID)
{
	std::string message;
	message="\3[Chat] "+users[pid].getUsername()+": "+(connData[threadID].buffer+1);
	sendData(-1,message.c_str(), (int)message.length());
}
void SocketServer::sendInitData(int gid)
{
	GameInitData init=games[gid].getInitData();
	//int test=session->getNumPlayers(); // getNumPlayers is removed
	for (int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		init.player[i].position=session->accessPlayer(i).getOrientation();
	}
	char* gd=new char[sizeof(init)+1];
	gd[0]=2;
	for (int i=0; i<games[gid].getPlayerCount(); i++)
	{
		int c=sizeof(init);
		init.pid=i;
		memcpy(gd+1,&init, sizeof(init));
		sendData(games[gid].getUserID(i), gd, sizeof(init)+1);
	}
}


void SocketServer::sendLobbyInitData(int lid)
{
	LobbyInitData init=lobby.getLobbyInitData();
	init.timer=LOBBY_WAIT_TIME;
	int c=sizeof(init);
	char* gd=new char[c+1];
	gd[0]=5;
	for (int i=0; i<lobby.getNrPlayers(); i++)
	{
		init.pid=i;
		memcpy(gd+1,&init, c);
		sendData(lobby.getUserID(i), gd, c+1);
	}
}