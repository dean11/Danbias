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
	int errorCode;

	char msgRecv[255] = "\0";

	InitWinSock();

	cout << "Client" << endl;

	//Create Client
	NetworkClient client;

	//Connect to server
	if(client.Connect(15151, "localhost"))
	{
		cout << "FAILED" << endl;
	}
	client.SetRecieverObject(proc, NetworkProtocolCallbackType_Function);

	cout << "Done" << endl;

	while(1)
	{

	}

	ShutdownWinSock();

	system("pause");
	return 0;
}