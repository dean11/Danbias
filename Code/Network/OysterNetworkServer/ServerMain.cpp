#include <iostream>
#include <WinSock2.h>
#include "Listener.h"
#include "Client.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")

void ShutdownSockets();
bool InitSockets();

int main()
{
	cout << "Server" << endl;

	if(!InitSockets())
	{
		cout << "Sockets failed to initialize" << endl;
	}

	//Create socket
	Listener listener;
	listener.Init(9876);

	//Start listening
	int clientSocket = listener.Accept();

	//Accept a client
	Client client(clientSocket);

	//Send a message to that client
	client.Send("asd");

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