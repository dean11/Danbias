#include <iostream>
#include "Client.h"
#include <WinSock2.h>
using namespace std;
using namespace Oyster::Network::Client;
#pragma comment(lib, "ws2_32.lib")

void ShutdownSockets();
bool InitSockets();
void chat(Client client);

#include "../NetworkDependencies/Protocols.h"
#include "../NetworkDependencies/Translator.h"
using namespace Oyster::Network::Protocols;

int main()
{
	char msgRecv[255] = "\0";

	InitSockets();

	cout << "Client" << endl;

	//Create Client
	Client client;

	//Connect to server
	client.Connect(9876, "127.0.0.1");

	unsigned char* recvBuffer = new unsigned char[255];

	client.Send(recvBuffer);

	//chat(client);

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
	unsigned char msgRecv[255] = "\0";
	unsigned char msgSend[255] = "\0";

	bool chatDone = false;

	while(!chatDone)
	{
		client.Recv(msgRecv);

		cout<< "Client 2: " << msgRecv << endl;

		cin.getline((char*)msgSend , 255 , '\n');

		if(strlen((char*)msgSend) < 1)
		{
			memcpy(msgSend, " ", 1);
		}

		if((char*)msgSend != "exit")
		{
			if(strlen((char*)msgSend) < 1)
			{
				memcpy(msgSend, "ERROR", 1);
			}
			client.Send(msgSend);
		}

		else
		{
			chatDone = true;
		}

		cin.clear();

	}

}