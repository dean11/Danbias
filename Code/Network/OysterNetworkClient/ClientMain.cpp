#include <iostream>
#include <WinSock2.h>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "..\NetworkDependencies\Translator.h"
#include "..\NetworkDependencies\Protocols.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../../Misc/ThreadSafeQueue.h"
#include "Client.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace Oyster::Network::Protocols;
using namespace Oyster::Network::Client;

void chat(Client &client);

int main()
{
	int errorCode;

	char msgRecv[255] = "\0";

	InitWinSock();

	cout << "Client" << endl;

	//Create Client
	Client client;

	//Connect to server
	errorCode = client.Connect(9876, "localhost");

	if(errorCode != 0)
	{
		wstring errorTest = GetErrorMessage(errorCode);
		wcout << "errorMessage: " << errorTest << endl;
	}

	chat(client);

	ShutdownWinSock();

	system("pause");
	return 0;
}

void chat(Client &client)
{
	Oyster::Network::Translator *t = new Oyster::Network::Translator();

	Oyster::Network::OysterByte msgRecv;
	string msgSend = "";

	ProtocolSet* set = new ProtocolSet;
	ProtocolPlayerPos test;
	test.ID = 5;
	test.matrix = new float[16];
	float temp = 10;
	for(int i = 0; i < 5; i++)
	{
		test.matrix[i] = temp;
		temp++;
	}

	bool chatDone = false;

	while(!chatDone)
	{
		client.Recv(msgRecv);
		
		t->Unpack(set, msgRecv);
		
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
			cout << "Server: ID " << set->Protocol.pPlayerPos->ID << endl;
			for(int i = 0; i < set->Protocol.pPlayerPos->nrOfFloats; i++)
			{
				cout << set->Protocol.pPlayerPos->matrix[i] << ' ';
			}
			cout << endl;
			break;
		}
		
		set->Release();
		msgRecv.Clear(1000);

		/*std::getline(std::cin, msgSend);


	
		if( msgSend != "exit")
		{
			if(msgSend.length() < 1)
			{
				msgSend = "ERROR!";
			}

			test.textMessage = msgSend;
			
			t->Pack(test, msgRecv);

			client.Send(msgRecv);
		}

		else
		{
			chatDone = true;
		}

		cin.clear();*/

	}

	delete t;
	delete set;
}