#include <iostream>
#include "Client.h"
#include <WinSock2.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void ShutdownSockets();
bool InitSockets();

int main()
{
	char msgRecv[255] = "\0";

	InitSockets();

	cout << "Client" << endl;

	//Create Client
	Client client;

	//Connect to server
	client.Connect(9876, "127.0.0.1");

	//Recieve message
	client.Recv(msgRecv);

	//print message
	cout << msgRecv << endl;

	ShutdownSockets(); 

	system("pause");
	return 0;
}

bool InitSockets()
{
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
}

void ShutdownSockets()
{
	WSACleanup();
}