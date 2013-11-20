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

	char recvBuffer[255];

	if(!InitSockets())
	{
		cout << "Sockets failed to initialize" << endl;
	}

	//Create socket
	Listener listener;
	listener.Init(9876);

	//Start listening
	//Accept a client
	int clientSocket = listener.Accept();
	Client client1(clientSocket);
	cout << "First client connected." << endl;

	//Accept a client
	clientSocket = listener.Accept();
	Client client2(clientSocket);
	cout << "Second client connected." << endl;

	client1.Send("Hej");

	while(1)
	{
		client1.Recv(recvBuffer);
		cout << "Client1: " << recvBuffer << endl;
		client2.Send(recvBuffer);

		client2.Recv(recvBuffer);
		cout << "Client2: " << recvBuffer << endl;
		client1.Send(recvBuffer);
	}

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