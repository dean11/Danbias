#include <iostream>
#include <WinSock2.h>
#include <vector>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "TestClass.h"

using namespace std;

void clientProc(Oyster::Network::NetworkClient* client);

int main()
{
	if(!InitWinSock())
	{
		cout << "errorMessage: unable to start winsock" << endl;
	}

	Test test;

	cout << "Server" << endl;

	test.mainLoop();

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
	}

	Sleep(1000);

	system("pause");

	return 0;
}