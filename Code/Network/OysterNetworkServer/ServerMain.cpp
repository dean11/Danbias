#include <iostream>
#include <WinSock2.h>
#include <vld.h>
#include "../NetworkDependencies/Listener.h"
#include "Client.h"
#include "../NetworkDependencies/Packing.h"
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
	unsigned char* recvBuffer = new unsigned char[5000];
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
	test.numOfFloats = 500;
	test.f = new float[test.numOfFloats];
	float temp = 500.456f;
	for(int i = 0; i < test.numOfFloats; i++)
	{
		test.f[i] = temp;
		temp--;
	}
	recvBuffer = t.Pack(test);

	client1.Send(recvBuffer);
	
	while(1)
	{
		client1.Recv(recvBuffer);
		
		t.Unpack(set, recvBuffer);
		cout << set->Protocol.pTest->clientID << ' ' << set->Protocol.pTest->packageType << ' ' << set->Protocol.pTest->size << endl;
		cout << "Client1: " << set->Protocol.pTest->textMessage << endl;
		for(int i = 0; i < set->Protocol.pTest->numOfFloats; i++)
		{
			cout << set->Protocol.pTest->f[i] << ' ';
		}
		cout << endl;
		set->Release();
		client2.Send(recvBuffer);

		client2.Recv(recvBuffer);

		t.Unpack(set, recvBuffer);
		cout << set->Protocol.pTest->clientID << ' ' << set->Protocol.pTest->packageType << ' ' << set->Protocol.pTest->size << endl;
		cout << "Client2: " << set->Protocol.pTest->textMessage << endl;
		for(int i = 0; i < set->Protocol.pTest->numOfFloats; i++)
		{
			cout << set->Protocol.pTest->f[i] << ' ';
		}
		cout << endl;
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