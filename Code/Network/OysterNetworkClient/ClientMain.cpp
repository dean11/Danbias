#include <iostream>
#include "Client.h"
#include <WinSock2.h>
#include "..\NetworkDependencies\Translator.h"

using namespace std;
using namespace Oyster::Network::Protocols;;
using namespace Oyster::Network::Client;
#pragma comment(lib, "ws2_32.lib")


void ShutdownSockets();
bool InitSockets();
void chat(Client client);

#include "../NetworkDependencies/Protocols.h"
#include "../NetworkDependencies/Translator.h"
using namespace Oyster::Network::Protocols;

int main()
{
	char msgRecv[255] = "\0";

	InitSockets();

	cout << "Client" << endl;

	//Create Client
	Client client;

	//Connect to server
	client.Connect(9876, "127.0.0.1");

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

void chat(Client client)
{
	Oyster::Network::Translator *t = new Oyster::Network::Translator();

	unsigned char msgRecv[255] = "\0";
	string msgSend = "";

	ProtocolHeader* header;
	ProtocolTest test;

	bool chatDone = false;

	while(!chatDone)
	{
		client.Recv(msgRecv);
		
		header = t->Translate(msgRecv);

		switch(header->packageType)
		{
		case package_type_header:
			break;
		case package_type_test:
			cout <<"Client 2: " <<((ProtocolTest*)&header)->textMessage <<endl;
			break;
		}


		std::getline(std::cin, msgSend);
		std::cin.clear();

		if(msgSend.length() < 1)
		{
			//memcpy(msgSend, " " , 1);
			msgSend = " ";
			//strcpy_s((char)msgSend , " ");
		}

		if( msgSend != "exit")
		{
			if(msgSend.length() < 1)
			{
				//memcpy(msgSend, "ERROR" , 5);
				msgSend = "ERROR!";
				//strcpy_s(msgSend, "ERROR");
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

}