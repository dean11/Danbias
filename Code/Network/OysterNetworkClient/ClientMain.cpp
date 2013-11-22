#include <iostream>
#include "Client.h"
#include <WinSock2.h>
#include <vld.h>
#include "..\NetworkDependencies\Translator.h"
#include "..\NetworkDependencies\Protocols.h"

using namespace std;
using namespace Oyster::Network::Protocols;;
using namespace Oyster::Network::Client;

#pragma comment(lib, "ws2_32.lib")

void ShutdownSockets();
bool InitSockets();
void chat(Client &client);

int main()
{
	char msgRecv[255] = "\0";

	InitSockets();

	cout << "Client" << endl;

	//Create Client
	Client client;

	//Connect to server
	client.Connect(9876, "10.0.0.3");

	chat(client);

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

void chat(Client &client)
{
	Oyster::Network::Translator *t = new Oyster::Network::Translator();

	unsigned char msgRecv[256] = "\0";
	string msgSend = "";

	ProtocolSet* set = new ProtocolSet;
	ProtocolHeader* header;
	ProtocolTest test;

	bool chatDone = false;

	while(!chatDone)
	{
		client.Recv(msgRecv);
		
		t->Translate(set, msgRecv);
		
		switch(set->t)
		{
		case package_type_header:
			break;
		case package_type_test:
			cout <<"Client 2: " << set->Protocol.pTest->textMessage <<endl;
			break;
		}
		
		set->Release();

		std::getline(std::cin, msgSend);
	
		if( msgSend != "exit")
		{
			if(msgSend.length() < 1)
			{
				msgSend = "ERROR!";
			}

			test.packageType = package_type_test;
			test.size = msgSend.length();
			test.textMessage = msgSend;
			
			unsigned char *message = t->Translate(test);

			client.Send(message);
		}

		else
		{
			chatDone = true;
		}

		cin.clear();

	}

	delete t;
	delete set;
}