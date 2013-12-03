#include <iostream>
#include <WinSock2.h>
#include <vector>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "../NetworkDependencies/Listener.h"
#include "../NetworkDependencies/Translator.h"
#include "../NetworkDependencies/ThreadedClient.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../NetworkDependencies/PostBox.h"
#include "../../Misc/WinTimer.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace Oyster::Network::Server;
using namespace Oyster::Network;
using namespace ::Protocols;
using namespace Utility;

int main()
{
	OysterByte recvBuffer;
	IPostBox<int>* postBox = new PostBox<int>();

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
	listener.SetPostBox(postBox);
	Sleep(1000);

	//Start listening
	//Accept a client
	ProtocolPlayerPos test;
	test.clientID = 0;
	test.ID = 5;
	test.nrOfFloats = 16;
	test.matrix = new float[test.nrOfFloats];

	for(int i = 0; i < (int)test.nrOfFloats; i++)
	{
		test.matrix[i] = (float)i;
	}

	t.Pack(test, recvBuffer);
	
	WinTimer timer;

	vector<ThreadedClient*> clients;
	int client = -1;
	while(1)
	{
		client = -1;
		postBox->FetchMessage(client);
		if(client != -1)
		{
			cout << "Client connected: " << client << endl;
			clients.push_back(new ThreadedClient(client));

			clients.at(clients.size()-1)->Send(&recvBuffer);
		}

		//Send a message every 1 secounds to all clients.
		if(timer.getElapsedSeconds() > 1)
		{
			cout << "Sending to " << clients.size() << " clients." << endl;
			timer.reset();
			for(int i = 0; i < (int)clients.size(); i++)
			{
				clients.at(i)->Send(&recvBuffer);
			}
		}
		Sleep(100);
	}
	listener.Shutdown();

	/*
	ProtocolSet* set = new ProtocolSet;

	client1.Send(recvBuffer);

	while(1)
	{
		client1.Recv(recvBuffer);

		t.Unpack(set, recvBuffer);
		cout << set->Protocol.pTest->clientID << ' ' << set->Protocol.pTest->packageType << ' ' << set->Protocol.pTest->size << endl;
		cout << "Client1: " << set->Protocol.pTest->textMessage << endl;
		for(int i = 0; i < (int)set->Protocol.pTest->numOfFloats; i++)
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
		for(int i = 0; i < (int)set->Protocol.pTest->numOfFloats; i++)
		{
			cout << set->Protocol.pTest->f[i] << ' ';
		}
		cout << endl;
		set->Release();
		client1.Send(recvBuffer);
	}


	ShutdownWinSock();
	delete set;
	*/
	system("pause");
	return 0;
}
