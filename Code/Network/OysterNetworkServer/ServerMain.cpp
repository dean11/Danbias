#include <iostream>
#include <vector>
#include <vld.h>
#include <mutex>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "../NetworkAPI/NetworkServer.h"
#include "../NetworkAPI/CustomNetProtocol.h"
#include "../NetworkAPI/NetworkCallbackHelper.h"

using namespace Oyster::Network;
using namespace std;

std::mutex m;
vector<NetworkClient> clients;

void proc(NetworkClient client)
{
	cout << "Hej" << endl;
	m.lock();
	clients.push_back(client);
	m.unlock();
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
		Sleep(1000);
		m.lock();
		cout << clients.size() << endl;
		m.unlock();
		break;
	}

	server.Stop();

	system("pause");

	return 0;
}