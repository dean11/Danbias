#pragma once
//Start by defining unicode
//#ifndef UNICODE
//#define UNICODE
//#endif
//defining WIN32_LEAN_AND_MEAN this early is REQUIRED if you want to avoid a certain winsock error.
//#define WIN32_LEAN_AND_MEAN
//#define NOMINMAX
//#include 
//#include "GameClassExample.h"
//These includes are required for winsock
#include "Network.h"
//#include <winsock2.h>
//#include <Ws2tcpip.h>
//#include <stdio.h>
//#include <windows.h>
//#include "OysterMath.h"
//These are optional includes for various useful features
#include <time.h>
#include <string>
#include <ctime>
#include <iostream>

//ws2_32.lib is a lib file the linker requires for winsock compilation
#pragma comment(lib, "Ws2_32.lib")

//constants used by the socket client to avoid hard coding and/or mass variable declaration
const short TCPSendPort = 11110;
const short TCPRecvPort = 11111;
const short UDPSendPort = 11000;
const short UDPRecvPort = 11001;
const int BUFFER_MAX_SIZE = 4096;

enum ConnectionStatus
{
	OFFLINE,
	ONLINE_MAINMENU,
	ONLINE_QUEUEING,
	ONLINE_INLOBBY,
	ONLINE_INGAME
};
class SocketClient
{
private:
	HANDLE threadhandle[2];
	int sendDelayMS;

	//2 bools used to verify the activation of the client so threads can't start too early
	ConnectionStatus connStatus;
	bool setupStatus;
	bool connectStatus;

	//iResult is used to check error codes
	int iResult;
	//wsaData records error messages and errors which winsock might encounter
	WSADATA wsaData;

	//Main socket
	SOCKET connUDP;
	SOCKET connTCP;

	//Addresses used for data transfer
	sockaddr_in TCPrecvAddr;
	sockaddr_in TCPsendAddr;
	//UDPrecvAddr marks the port and IP adress the server is supposed to return data to.
	sockaddr_in UDPrecvAddr;
	//UDPsendAddr marks which IP and port the client is supposed to send data to.
	sockaddr_in UDPsendAddr;
	//size of a sockaddr_in. This might as well be a constant, but i'm keeping it in the class for performance reasons.
	int addrSize;

	//buffer which is filled when data receive happens.
	char* recvBuffer;
	//this variable tracks the buffer length.
	int recvBufLen;

	//dataBuf is a buffer solely for sending your own user data. It never changes size in order to increase performance.
	//char* sendBuffer;
	//int sendBufLen;
	//PlayerStruct tmpPlayer;
	char* dataBuf;
	int playerDataSize;
public:
	void setPlayerDataSize(int pds){playerDataSize=pds;}
	//Constructor
	SocketClient();

	//Initiation for sockets.
	bool init(int listenPort);
	bool initTCPSocket(int listenPort);
	bool initUDPSocket(int listenPort);
	//Connects to a server of a user-defined IP. Can only be called after an initXSocket has gone through.
	//The 2 remaining variables are init data and size of said data. Currently username.
	bool connectToIP(const char* ip, int listenPort, char* initData, int initDataSize);
	//sends an undefined data type of (variable#2) size to the server.
	bool sendDataUDP(const char*, int);
	bool sendDataTCP(const char*, int);
	//sends a text string to the server.
	bool sendMessage(std::string str);
	bool sendServerMessage(std::string str);
	//sends user data to the server
	bool sendUserData();
	bool sendUserData(char* data, int size);

	//Closes connection, kills off the socket.
	bool closeConnection();

	//Simple ifBoolIsTrue checks
	bool isReady() const {return setupStatus;}
	bool isConnected() const {return connectStatus;}
	void receiveDataWaitOnResponse();
	//Sends data periodically
	static void sendDataThreadV(SocketClient* ptr);
	//Receive loop. This is event-based and is on its own thread.
	static void receiveDataThreadV(SocketClient* ptr);
	//Once data is received, it calls on the parseReceivedData function.
	void parseReceivedData();
	//void parseReceivedKeyframe();
	//If an event is called from the server, this function will be called.
	void parseReceivedEvent();
	void parseReceivedEffect();
	//It is then sent to one of the following functions based on the first byte of the buffer.

	//Servermessage
	void parseServermessage();
	//single user data
	void parseData();
	//string (character data)
	void parseMessage();
	//init data which sets the start position etc of all characters.
	void parseLobbyInitData();
	void parseGameInitData();
	void parseRenderData();

	bool startReceiveThread();
	bool startSendDataThread();
	void setSendDelay(int ms){sendDelayMS=ms;}

	//virtual functions
	virtual void receiveGameInitData(char*)=0;
	virtual void receiveLobbyInitData(char*, int)=0;
	virtual void receivePlayerUpdate(char*, int)=0;
	virtual void receiveRenderData(char*, int)=0;
	virtual void receiveEffectData(char*, int)=0;
	virtual void receiveConnStatus(ConnectionStatus)=0;
	virtual void receiveEvent(char*)=0;
};