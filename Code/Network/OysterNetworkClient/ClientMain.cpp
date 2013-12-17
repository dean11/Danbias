#include <iostream>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"
#include "..\NetworkDependencies\Protocols.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../../Misc/WinTimer.h"
#include "../../Misc/Utilities.h"
#include "../NetworkAPI/NetworkClient.h"

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
	errorCode = client.Connect(15151, "193.11.187.62");
	client.SetRecieverObject(proc, NetworkProtocolCallbackType_Function);

	if(errorCode != 1)
	{
		printf("%d", errorCode);
		cout << "FAILED" << endl;
	}
	//client.SetRecieverObject(proc, NetworkProtocolCallbackType_Function);

	OysterByte bytes, bytes2, bytes3;
	bytes.AddSize(4);
	bytes2.AddSize(5);
	strcpy_s(bytes, 5, "asda");
	strcpy_s(bytes2, 6, "asdab");
	bytes += bytes2;
	bytes += bytes;
	bytes += bytes;
	bytes3 = bytes;

	cout << "Done" << endl;

	while(1)
	{
		
	}

	ShutdownWinSock();

	system("pause");
	return 0;
}