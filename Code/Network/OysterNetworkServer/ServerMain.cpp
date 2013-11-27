#include <iostream>
#include <WinSock2.h>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "../NetworkDependencies/Listener.h"
#include "../NetworkDependencies/Translator.h"
#include "Client.h"
#include "../NetworkDependencies/OysterByte.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace Oyster::Network::Server;
using namespace Oyster::Network;
using namespace ::Protocols;

int main()
{
	OysterByte recvBuffer;

	cout << "Server" << endl;
	Translator t;
	int errorCode;

	if(!InitWinSock())
	{
		cout << "errorMessage: unable to start winsock" << endl;
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
	test.size = 2;
	test.textMessage = "hej";
	test.numOfFloats = 35;
	test.f = new float[test.numOfFloats];
	float temp = 395.456f;
	for(int i = 0; i < test.numOfFloats; i++)
	{
		test.f[i] = temp;
		temp--;
	}

	t.Pack(test, recvBuffer);

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


	ShutdownWinSock();
	delete set;

	system("pause");
	return 0;
}
