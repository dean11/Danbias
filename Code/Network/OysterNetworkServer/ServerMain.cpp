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

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace Oyster::Network::Server;
using namespace Oyster::Network;
using namespace ::Protocols;
using namespace Utility;
using namespace ::Utility::DynamicMemory;

void PrintOutMessage(ProtocolSet* set);

int main()
{ 
	SmartPointer<OysterByte> sendBuffer = SmartPointer<OysterByte>(new OysterByte);
	SmartPointer<OysterByte> recvBuffer = SmartPointer<OysterByte>(new OysterByte());
	ProtocolSet* set = new ProtocolSet;
	IPostBox<SmartPointer<int>> *postBox = new PostBox<SmartPointer<int>>();
	IPostBox<SmartPointer<OysterByte>> *recvPostBox = new PostBox<SmartPointer<OysterByte>>();

	cout << "Server" << endl;
	Translator t;
	int errorCode = 0;

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
	test.nrOfFloats = 10;
	test.matrix = new float[test.nrOfFloats];

	for(int i = 0; i < (int)test.nrOfFloats; i++)
	{
		test.matrix[i] = (float)i;
	}

	t.Pack(test, sendBuffer);
	
	WinTimer timer;

	vector<ThreadedClient*> clients;
	SmartPointer<int> client = SmartPointer<int>();
	while(1)
	{
		//Fetch new clients from the postbox
		if(postBox->FetchMessage(client))
		{
			cout << "Client connected: " << *client << endl;
			clients.push_back(new ThreadedClient(recvPostBox, *client));

			clients.at(clients.size()-1)->Send(sendBuffer);
		}

		//Send a message every 1 secounds to all clients.
		if(timer.getElapsedSeconds() > 1)
		{
			cout << "Sending to " << clients.size() << " clients." << endl;
			timer.reset();
			for(int i = 0; i < (int)clients.size(); i++)
			{
				clients.at(i)->Send(sendBuffer);
			}
		}

		//Fetch messages
		if(recvPostBox->FetchMessage(recvBuffer))
		{
			t.Unpack(set, recvBuffer);

			PrintOutMessage(set);
			set->Release();
		}

		Sleep(1);
	}
	listener.Shutdown();
	Sleep(1000);

	system("pause");

	for(int i = 0; i < clients.size(); i++)
		delete clients.at(i);

	delete postBox;
	return 0;
}

void PrintOutMessage(ProtocolSet* set)
{
	switch(set->type)
	{
	case PackageType_header:
		break;
	case PackageType_test:
		cout <<"Client 2: " << set->Protocol.pTest->textMessage <<endl;
		for(int i = 0; i < (int)set->Protocol.pTest->numOfFloats; i++)
		{
			cout << set->Protocol.pTest->f[i] << ' ' ;
		}
		cout << endl;
		break;

	case PackageType_player_pos:
		//cout << "ID " << set->Protocol.pPlayerPos->ID << endl;
		for(int i = 0; i < (int)set->Protocol.pPlayerPos->nrOfFloats; i++)
		{
			cout << set->Protocol.pPlayerPos->matrix[i] << ' ';
		}
		cout << endl;
		break;
	}
}