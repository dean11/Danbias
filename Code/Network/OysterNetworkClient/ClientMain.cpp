#include <iostream>
#include <WinSock2.h>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "..\NetworkDependencies\Protocols.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../../Misc/ThreadSafeQueue.h"
#include "../NetworkDependencies/ThreadedClient.h"
#include "../../Misc/WinTimer.h"
#include "../../Misc/Utilities.h"
#include "../NetworkAPI/NetworkClient.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace Oyster::Network::Protocols;
using namespace Oyster::Network;
using namespace Utility;
using namespace Utility::DynamicMemory;

void proc(CustomNetProtocol& protocol)
{

}

int main()
{
	SetDllDirectory("..\\DLL\\");
	int errorCode;

	char msgRecv[255] = "\0";

	InitWinSock();

	cout << "Client" << endl;

	//Create Client
	NetworkClient client;

	//Connect to server
	//errorCode = client->Connect(15151, "193.11.186.101");
	errorCode = client.Connect(15151, "127.0.0.1");
	client.SetRecieverObject(proc, NetworkProtocolCallbackType_Function);

	if(errorCode != 1)
	{
		printf("%d", errorCode);
		cout << "FAILED" << endl;
	}
	//client.SetRecieverObject(proc, NetworkProtocolCallbackType_Function);

	cout << "Done" << endl;

	while(1)
	{

	}

	ShutdownWinSock();

	system("pause");
	return 0;
}