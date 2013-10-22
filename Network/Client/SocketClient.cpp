#include "SocketClient.h"
#pragma once
#ifndef SOCKET_CLIENT_CPP
#define SOCKET_CLIENT_CPP

SocketClient::SocketClient()
{
	playerDataSize=Network::CLIENT_PLAYER_DATA_SIZE;
	sendDelayMS=10;
	connUDP = INVALID_SOCKET;
	connTCP = INVALID_SOCKET;
	//sendBuffer=new char[BUFFER_MAX_SIZE];
	//sendBufLen=BUFFER_MAX_SIZE;
	//ZeroMemory(sendBuffer,sendBufLen);
	recvBuffer=new char[BUFFER_MAX_SIZE];
	recvBufLen=BUFFER_MAX_SIZE;
	ZeroMemory(recvBuffer,recvBufLen);

	dataBuf=new char[playerDataSize+1];
	dataBuf[0]=1;
	//ZeroMemory(b,sizeof(buffer));
	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup failed with error: %d\n", iResult);
	}


	
	addrSize=sizeof(sockaddr_in);
	connectStatus=false;
}



bool SocketClient::sendUserData()
{
	//memcpy(dataBuf+1,&playerContPtr->getPlayerData(),playerDataSize);
	//return sendData(dataBuf, playerDataSize+1);
	printf("NOT YET IMPLEMENTED");
	return false;
}

bool SocketClient::sendUserData(char* data, int size)
{
	memcpy(dataBuf+1,data,size);
	return sendDataUDP(dataBuf, size+1);
}

bool SocketClient::sendMessage(std::string msg)
{
	if (msg[0]=='/')
	{
		//Server command
		msg[0]=2;

	}
	else
	{
		//Chat message
		msg='1'+msg;
		msg[0]=3;
	}
	return sendDataUDP(msg.c_str(), (int)msg.size());
}

bool SocketClient::closeConnection()
{
	connectStatus=false;
	Sleep(5);
	//Give the threads 5 ms to quit themselves before terminating them
	DWORD eCode=0;
	TerminateThread(threadhandle[0], eCode);
	TerminateThread(threadhandle[1], eCode);
	//---------------------------------------------
	// When the application is finished sending, close the socket.
	setupStatus=false;
	printf("Finished sending. Closing socket.\n");
	iResult = closesocket(connUDP);
	if (iResult == SOCKET_ERROR) 
	{
		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	//---------------------------------------------
	// Clean up and quit.
	printf("Exiting.\n");
	WSACleanup();
	return true;
}

void SocketClient::receiveDataThreadV(SocketClient* ptr)
{
	while(true)
	{
		ptr->recvBufLen=recvfrom(ptr->connUDP, ptr->recvBuffer, BUFFER_MAX_SIZE, 0, (SOCKADDR *) & ptr->UDPsendAddr, &ptr->addrSize);
		if (ptr->recvBufLen == SOCKET_ERROR) 
		{
			wprintf(L"recv failed with error %d\n", WSAGetLastError());
		}
		//ptr->buffer[ptr->iResult]='\0';
		else
			ptr->parseReceivedData();
	}
}


void SocketClient::receiveDataWaitOnResponse()
{
	recvBufLen=recvfrom(connUDP, recvBuffer, BUFFER_MAX_SIZE, 0, (SOCKADDR *) & UDPsendAddr, &addrSize);
	if (recvBufLen == SOCKET_ERROR) 
	{
		wprintf(L"recv failed with error %d\n", WSAGetLastError());
	}
	//buffer[iResult]='\0';
	else
		parseReceivedData();
}

void SocketClient::sendDataThreadV(SocketClient* ptr)
{
	printf("NOT YET IMPLEMENTED");
	/*while(ptr->connectStatus)
	{
		memcpy(ptr->dataBuf+1,&ptr->playerContPtr->getPlayerData(),playerDataSize);
		ptr->sendData(ptr->dataBuf, playerDataSize+1);
		Sleep(ptr->sendDelayMS);
	}*/
}

#endif