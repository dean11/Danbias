#include "TestClass.h"
#include "../../Misc/WinTimer.h"
#include <iostream>

using namespace Oyster::Network;
using namespace ::Protocols;
using namespace Utility;
using namespace ::DynamicMemory;
using namespace std;

Test::Test()
{
	recvPostBox = new PostBox<SmartPointer<OysterByte>>;
}

Test::~Test()
{
	for(int i = 0; i < (int)clients.size(); i++)
		delete clients.at(i);
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