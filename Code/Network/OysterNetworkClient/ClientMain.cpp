#include <iostream>
#include "Client.h"
#include <WinSock2.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void ShutdownSockets();
bool InitSockets();
void chat(Client client);

int main()
{
	char msgRecv[255] = "\0";

	InitSockets();

	cout << "Client" << endl;

	//Create Client
	Client client;

	//Connect to server
	client.Connect(9876, "10.0.0.3");


	chat(client);

	//Recieve message
	//client.Recv(msgRecv);

	//print message
	//cout << msgRecv << endl;

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

void chat(Client client)
{
	char msgRecv[255] = "\0";
	char msgSend[255] = "\0";

	bool chatDone = false;

	while(!chatDone)
	{
		client.Recv(msgRecv);

		cout<< "Client 2: " << msgRecv << endl;

		cin.getline(msgSend , 255 , '\n');

		if(strlen(msgSend) < 1)
		{
			strcpy_s(msgSend , " ");
		}

		if(msgSend != "exit")
		{
			client.Send(msgSend);
		}

		else
		{
			chatDone = true;
		}

		cin.clear();

	}

}