#include "TestClass.h"
#include "../../Misc/WinTimer.h"
#include <iostream>

using namespace Oyster::Network;
using namespace ::Server;
using namespace ::Protocols;
using namespace Utility;
using namespace ::DynamicMemory;
using namespace std;

Test::Test()
{
	recvPostBox = new PostBox<SmartPointer<OysterByte>>;

	sendBuffer = new OysterByte;
	recvBuffer = new OysterByte;

	NetworkServer::INIT_DESC initDesc;
	initDesc.port = 9876;
	initDesc.proc = NULL;
	server.Init(initDesc);
	server.Start();

	test.clientID = 0;
	test.ID = 5;
	test.nrOfFloats = 10;
	test.matrix = new float[test.nrOfFloats];

	for(int i = 0; i < (int)test.nrOfFloats; i++)
	{
		test.matrix[i] = (float)i;
	}

	t.Pack(test, sendBuffer);
}

Test::~Test()
{
	for(int i = 0; i < (int)clients.size(); i++)
		delete clients.at(i);

	server.Stop();
}

void Test::ProcFunc(Utility::DynamicMemory::SmartPointer<Oyster::Network::OysterByte> msg)
{

	return;
}

void Test::mainLoop()
{
	WinTimer timer;

	while(1)
	{
		
	}
}

void Test::PrintOutMessage(ProtocolSet* set)
{
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
		//cout << "ID " << set->Protocol.pPlayerPos->ID << endl;
		for(int i = 0; i < (int)set->Protocol.pPlayerPos->nrOfFloats; i++)
		{
			cout << set->Protocol.pPlayerPos->matrix[i] << ' ';
		}
		cout << endl;
		break;
	}
}