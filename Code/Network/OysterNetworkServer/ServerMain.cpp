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
#include "../../Misc/Utilities.h"
#include "../../Misc/Utilities-Impl.h"

#include "TestClass.h"
#include "IServer.h"
#include "IClient.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace Oyster::Network::Server;
using namespace Oyster::Network;
using namespace ::Protocols;
using namespace Utility;
using namespace ::Utility::DynamicMemory;

void clientProc(IClient* client);

int main()
{
	Test tests;

	SmartPointer<OysterByte> sendBuffer = new OysterByte;
	SmartPointer<OysterByte> recvBuffer = new OysterByte;
	ProtocolSet* set = new ProtocolSet;

	cout << "Server" << endl;
	Translator t;
	int errorCode = 0;

	if(!InitWinSock())
	{
		cout << "errorMessage: unable to start winsock" << endl;
	}

	IServer server;
	IServer::INIT_DESC initDesc;
	initDesc.port = 9876;
	initDesc.proc = clientProc;
	server.Init(initDesc);
	server.Start();

	Sleep(1000);

	//Create a test protocol
	ProtocolPlayerPos test;
	test.clientID = 0;
	test.ID = 5;
	test.nrOfFloats = 10;
	test.matrix = new float[test.nrOfFloats];

	for(int i = 0; i < (int)test.nrOfFloats; i++)
	{
		test.matrix[i] = (float)i;
	}

	t.Pack(test, sendBuffer);

	WinTimer timer;

	while(1)
	{
		//Fetch new clients from the postbox
		/*
		//Send a message every 1 seconds to all clients.
		if(timer.getElapsedSeconds() > 1)
		{
			cout << "Sending to " << clients.size() << " clients." << endl;
			timer.reset();
			for(int i = 0; i < (int)clients.size(); i++)
			{
				clients.at(i)->Send(sendBuffer);
			}
		}*/

		/*//Fetch messages
		if(recvPostBox->FetchMessage(recvBuffer))
		{
			t.Unpack(set, recvBuffer);

			//PrintOutMessage(set);
			set->Release();
		}*/

		Sleep(1);
	}
	server.Stop();
	server.Shutdown();
	//listener.Shutdown();
	Sleep(1000);

	system("pause");

	return 0;
}

void clientProc(IClient* client)
{
	cout << "Proc" << endl;
	clients.push_back(client);
}