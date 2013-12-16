#include <iostream>
#include <vector>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "../NetworkAPI/NetworkServer.h"

using namespace Oyster::Network;
using namespace std;

void proc(NetworkClient client)
{
	cout << "Hej" << endl;
}

int main()
{
	SetDllDirectory("..\\DLL\\");

	NetworkServer server;
	Oyster::Network::NetworkServer::INIT_DESC desc;
	desc.port = 15151;
	desc.callbackType = NetworkClientCallbackType_Function;
	desc.recvObj = proc;

	if(!server.Init(desc))
	{
		cout << "Init failed" << endl;
		return 0;
	}

	if(!server.Start())
	{
		cout << "Start failed" << endl;
		return 0;
	}

	cout << "Server started" << endl;

	while(1)
	{

	}


	system("pause");

	return 0;
}