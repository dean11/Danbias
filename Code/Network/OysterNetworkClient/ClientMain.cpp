#include <iostream>
#include <WinSock2.h>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "..\NetworkDependencies\Translator.h"
#include "..\NetworkDependencies\Protocols.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../../Misc/ThreadSafeQueue.h"
#include "../NetworkDependencies/ThreadedClient.h"
#include "../../Misc/WinTimer.h"
#include "../../Misc/Utilities.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace Oyster::Network::Protocols;
using namespace Oyster::Network;
using namespace Utility;

void chat(ThreadedClient &client);
void PrintOutMessage(ProtocolSet* set);

int main()
{
	int errorCode;

	char msgRecv[255] = "\0";

	InitWinSock();

	cout << "Client" << endl;

	//Create Client
	ThreadedClient* client = new ThreadedClient;

	//Connect to server
	errorCode = client->Connect(9876, "localhost");

	if(errorCode != 0)
	{
		wstring errorTest = GetErrorMessage(errorCode);
		wcout << "errorMessage: " << errorTest << endl;
	}

	chat(*client);
	delete client;

	ShutdownWinSock();

	system("pause");
	return 0;
}

void chat(ThreadedClient &client)
{
	Oyster::Network::Translator *t = new Oyster::Network::Translator();
	IPostBox<OysterByte*>* postBox = new PostBox<OysterByte*>;

	client.setRecvPostBox(postBox);

	Oyster::Network::OysterByte* msgRecv = NULL;
	Oyster::Network::OysterByte* msgSend = new OysterByte();

	ProtocolSet* set = new ProtocolSet;
	ProtocolPlayerPos test;
	test.ID = 5;
	test.nrOfFloats = 5;
	test.matrix = new float[test.nrOfFloats];
	float temp = 10;
	for(int i = 0; i < (int)test.nrOfFloats; i++)
	{
		test.matrix[i] = temp;
		temp++;
	}
	t->Pack(test, *msgSend);

	WinTimer timer;

	while(1)
	{
		//Fetch new messages from the postbox
		if(postBox->FetchMessage(msgRecv))
		{
			t->Unpack(set, *msgRecv);
			delete msgRecv;

			PrintOutMessage(set);
			set->Release();
		}

		//Send message to server each second
		if(timer.getElapsedSeconds() > 1)
		{
			cout << "Sending to server." << endl;
			timer.reset();
			client.Send(msgSend);
		}
	}

	delete msgSend;
	delete postBox;
	delete t;
	delete set;
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
		cout << "ID " << set->Protocol.pPlayerPos->ID << endl;
		for(int i = 0; i < (int)set->Protocol.pPlayerPos->nrOfFloats; i++)
		{
			cout << set->Protocol.pPlayerPos->matrix[i] << ' ';
		}
		cout << endl;
		break;
	}
}