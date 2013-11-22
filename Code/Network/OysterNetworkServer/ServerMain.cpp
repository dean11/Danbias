#include <iostream>
#include <WinSock2.h>
#include "Listener.h"
#include "Client.h"
using namespace std;
using namespace Oyster::Network::Server;

#pragma comment(lib, "ws2_32.lib")

void ShutdownSockets();
bool InitSockets();

#include "../NetworkDependencies/Translator.h"
using namespace Oyster::Network;
using namespace ::Protocols;

int main()
{
	unsigned char* recvBuffer = new unsigned char[255];
	cout << "Server" << endl;
	Translator t;

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

<<<<<<< HEAD
	client1.Send((unsigned char*)"Hej");
	ProtocolHeader* header = NULL;
=======
	client1.Send((unsigned char*) "Hej");
>>>>>>> 4142688f6c4a63aa97341205588ad6cace0f43af

	while(1)
	{
		client1.Recv(recvBuffer);

		header = t.Translate(recvBuffer);
		cout << header->clientID << ' ' << header->packageType << ' ' << header->size << endl;
		cout << "Client1: " << ((ProtocolTest*)header)->textMessage << endl;
		client2.Send(recvBuffer);

		client2.Recv(recvBuffer);

		header = t.Translate(recvBuffer);
		cout << header->clientID << ' ' << header->packageType << ' ' << header->size << endl;
		cout << "Client1: " << ((ProtocolTest*)header)->textMessage << endl;
		client1.Send(recvBuffer);
	}

	ShutdownSockets();

	delete[] recvBuffer;

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