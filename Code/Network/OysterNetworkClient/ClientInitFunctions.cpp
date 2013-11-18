#include "SocketClient.h"

#pragma once
#ifndef SOCKET_INIT_CPP
#define SOCKET_INIT_CPP

bool SocketClient::startReceiveThread()
{
	threadhandle[0]=CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)&receiveDataThreadV,
		(LPVOID) this,
		0,
		NULL);
	return true;
}

bool SocketClient::startSendDataThread()
{
	threadhandle[1]=CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)&receiveDataThreadV,
		(LPVOID) this,
		0,
		NULL);
	return true;
}
bool SocketClient::init(int listenPort)
{
	return initUDPSocket(listenPort);
}
bool SocketClient::connectToIP(const char* ip, int listenPort, char* initData, int initDataSize)
{
	init(listenPort);
	//---------------------------------------------
	// Set up the port and IP of the server
	//Port starts up as a different one from when connected, it changes once the server has exchanged some info with the client

	UDPsendAddr.sin_family = AF_INET;
	UDPsendAddr.sin_port = htons(UDPSendPort);
	UDPsendAddr.sin_addr.s_addr = inet_addr(ip);

	TCPsendAddr.sin_family = AF_INET;
	TCPsendAddr.sin_port = htons(TCPSendPort);
	TCPsendAddr.sin_addr.s_addr = inet_addr(ip);
	/*iResult=connect(connTCP, (SOCKADDR *) &TCPsendAddr, addrSize);
	if (iResult == SOCKET_ERROR) {
		int test=WSAGetLastError();
		wprintf(L"connect failed with error: %d\n", WSAGetLastError());
		//closesocket(connTCP);
		//WSACleanup();
		return false;
	}/*
	iResult=send(connTCP, initData, initDataSize, 0);
	if (iResult == SOCKET_ERROR) {
		int test=WSAGetLastError();
		wprintf(L"connect failed with error: %d\n", WSAGetLastError());
		//closesocket(connTCP);
		//WSACleanup();
		return false;
	}*/

	iResult = sendto(connUDP,
		initData, initDataSize, 0, (SOCKADDR *) & UDPsendAddr, addrSize);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"Client UDP sendto failed with error: %d\n", WSAGetLastError());
		//closesocket(connUDP);
		//WSACleanup();
		return false;
	}
	//connectStatus=true;
	connectStatus=false;
	return true;
}


#endif
