#include <iostream>
#include <WinSock2.h>
#include <vld.h>
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
	unsigned char* recvBuffer = new unsigned char[256];
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

	ProtocolSet* set = new ProtocolSet;
	ProtocolTest test;
	test.clientID = 0;
	test.textMessage = "hej";
	recvBuffer = t.Translate(test);

	client1.Send(recvBuffer);
	
	while(1)
	{
		client1.Recv(recvBuffer);
		
		t.Translate(set, recvBuffer);
		cout << set->Protocol.pTest->clientID << ' ' << set->Protocol.pTest->packageType << ' ' << set->Protocol.pTest->size << endl;
		cout << "Client1: " << set->Protocol.pTest->textMessage << endl;
		set->Release();
		client2.Send(recvBuffer);

		client2.Recv(recvBuffer);

		t.Translate(set, recvBuffer);
		cout << set->Protocol.pTest->clientID << ' ' << set->Protocol.pTest->packageType << ' ' << set->Protocol.pTest->size << endl;
		cout << "Client2: " << set->Protocol.pTest->textMessage << endl;
		set->Release();
		client1.Send(recvBuffer);
	}

	ShutdownSockets();
	delete[] recvBuffer;
	delete set;

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