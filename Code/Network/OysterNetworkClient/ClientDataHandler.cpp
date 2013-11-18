#include "SocketClient.h"

#pragma once
#ifndef SOCKET_DATA_CPP
#define SOCKET_DATA_CPP

/*std::vector<std::string> splitString(char* p_inStr, char p_delim)
{
std::stringstream ss(p_inStr);
std::vector<std::string> elems;
std::string item;
while(std::getline(ss, item, p_delim)) 
{
elems.push_back(item);
}
return elems;
}*/

void SocketClient::parseReceivedData(/*char* data, int size*/)
{
	switch (recvBuffer[0])	// TODO: runtime error occured here when shutting down client. recvBuffer invalid pointer. ~Dan 2013-05-14
	{
	case 1://It's data
		parseData();
		break;
	case 2://For the moment, this is only for init data
		parseGameInitData();
		break;
	case 3://It's a chat message
		parseMessage();
		break;
	case 4://It's a server message
		parseServermessage();
		break;
	case 5://Player has been connected to a game lobby
		parseLobbyInitData();
		break;
	case 6://It's an event
		parseReceivedEvent();
		break;
	case 7:
		parseReceivedEffect();
		break;
	case 8:
		parseRenderData();
		break;
	default:
		int a=0;
		
	}
}
void SocketClient::parseRenderData()
{
	receiveRenderData(recvBuffer+1, recvBufLen-1);
}
void SocketClient::parseReceivedEffect()
{
	receiveEffectData(recvBuffer+1, recvBufLen-1);
}
void SocketClient::parseReceivedEvent()
{
	receiveEvent(recvBuffer+1);
}
void SocketClient::parseGameInitData()
{
	receiveGameInitData(recvBuffer+1);
	connectStatus=true;
}

void SocketClient::parseLobbyInitData()
{
	receiveLobbyInitData(recvBuffer+1, recvBufLen-1);
	connectStatus=true;
}

void SocketClient::parseServermessage()
{
	recvBuffer[recvBufLen]='\0';
	if(!strcmp(recvBuffer+1, "connected"))
	{
		connectStatus=true;
		connStatus=ONLINE_MAINMENU;
		receiveConnStatus(ONLINE_MAINMENU);
	}
	else if(!strcmp(recvBuffer+1, "qst"))
	{
		connStatus=ONLINE_QUEUEING;
		receiveConnStatus(ONLINE_QUEUEING);
	}
	else if(!strcmp(recvBuffer+1, "qed"))
	{
		connStatus=ONLINE_MAINMENU;
		receiveConnStatus(ONLINE_MAINMENU);
	}
	//Server message of some sort
}

void SocketClient::parseData()
{
	//memcpy(&tmpPlayer,buffer+1,playerDataSize);
	//playerContPtr->setPlayerStruct(tmpPlayer);
	receivePlayerUpdate(recvBuffer+1, recvBufLen-1);
}

void SocketClient::parseMessage()
{
	//std::string message;
	//message="[Chat] "+users[pid].getUsername()+": "+(buffer+1);
	printf("%s\n",recvBuffer+1);
}

#endif
