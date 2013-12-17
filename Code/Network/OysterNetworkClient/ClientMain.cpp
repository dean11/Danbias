#include <iostream>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "..\NetworkDependencies\Protocols.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../../Misc/WinTimer.h"
#include "../../Misc/Utilities.h"
#include "../NetworkAPI/NetworkClient.h"

#include "..\..\Game\GameProtocols\PlayerProtocols.h"

using namespace std;
using namespace Oyster::Network;

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

	std::cout << "Done" << endl;

	GameLogic::Protocol_PlayerMovement p;
	while(1)
	{
		std::cout << ". ";
		client.Send(p);
		Sleep(100000);
	}

	ShutdownWinSock();

	system("pause");
	return 0;
}